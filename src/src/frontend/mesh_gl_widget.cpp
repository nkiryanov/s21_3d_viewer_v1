#include "frontend/mesh_gl_widget.hpp"

extern "C" {
#include "backend/loader.h"
#include "backend/polygon_t.h"
#include "backend/transformations.h"
#include "backend/vector_t.h"
}

#include <QDebug>
#include <cmath>
#include <unordered_set>

namespace ViewerFrontend {

MeshGLWidget::~MeshGLWidget() {
  makeCurrent();

  Cleanup();
  delete program;
}

void MeshGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  glEnable(GL_DEPTH_TEST);

  initShaders();
}

void MeshGLWidget::paintGL() {
  drawBackground();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Edge case. Do not draw the object if it not loaded
  if (mesh_state.is_loaded == false) return;

  CalculateMVPMatrix();

  program->bind();
  program->setUniformValue("mvp_matrix", mvp_matrix);

  vertex_array_object.bind();
  drawPointsIfNeeded();
  drawLines();
  vertex_array_object.release();

  program->release();
}

void MeshGLWidget::initShaders() {
  delete program;
  program = new QOpenGLShaderProgram();
  // Compile vertex shader
  if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                        ":/shaders/vertex.glsl"))
    close();

  // Compile fragment shader
  if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                        ":/shaders/fragment.glsl"))
    close();

  // Link shader pipeline
  if (!program->link()) close();

  // Bind shader pipeline for use
  if (!program->bind()) close();
}

void MeshGLWidget::initVertexBuffer() {
  vertex_buffer.create();
  vertex_buffer.bind();
  vertex_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vertex_buffer.allocate(mesh.vertices, sizeof(vector_t) * mesh.count_vertices);
  vertex_buffer.release();

  vertex_array_object.create();
  vertex_array_object.bind();

  vertex_buffer.bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);
  vertex_buffer.release();

  vertex_array_object.release();
}

void MeshGLWidget::initElementBuffer() {
  // Store all the polygons in a single element buffer array.
  // Since the number of polygons may vary, we draw the polygons using
  // `GL_LINES`. To achieve this, we explicitly break down the primitives into
  // lines, specifying the start and end points for each line. This approach
  // requires more memory but saves CPU cycles.

  std::vector<std::pair<GLuint, GLuint>> mesh_lines(0);
  std::unordered_set<std::string> existed_lines;

  // lambda to add line to mesh_lines if it not added already
  auto add_line_if_not_added =
      [](const std::pair<GLuint, GLuint>& line,
         std::unordered_set<std::string>& existed_lines,
         std::vector<std::pair<GLuint, GLuint>>& mesh_lines) -> void {
    std::string strait_line =
        std::to_string(line.first) + "-" + std::to_string(line.second);
    std::string opposite_line =
        std::to_string(line.second) + "-" + std::to_string(line.first);

    auto [iter_1, strait_inserted] = existed_lines.insert(strait_line);
    auto [iter_2, opposite_inserted] = existed_lines.insert(opposite_line);

    if (strait_inserted || opposite_inserted) mesh_lines.push_back(line);
  };

  for (uint32_t i = 0; i != mesh.count_polygons; ++i) {
    auto& polygon = mesh.polygons[i];

    GLuint first_index = 0;
    GLuint previous_index = 0;
    GLuint index = 0;

    for (uint32_t j = 0; j != polygon.count_indices; ++j) {
      index = (GLuint)polygon.vertex_indices[j];
      if (j != 0) {
        add_line_if_not_added({previous_index, index}, existed_lines,
                              mesh_lines);
      } else {
        first_index = index;
      }
      previous_index = index;
    }

    if (previous_index != first_index) {
      add_line_if_not_added({previous_index, first_index}, existed_lines,
                            mesh_lines);
    }
  }

  mesh_count_lines = mesh_lines.size();
  element_buffer.create();
  element_buffer.bind();
  element_buffer.allocate(mesh_lines.data(),
                          mesh_count_lines * sizeof(GLuint) * 2);
  element_buffer.release();
}

void MeshGLWidget::drawBackground() {
  glClearColor(mesh_state.background.redF(), mesh_state.background.greenF(),
               mesh_state.background.blueF(), mesh_state.background.alphaF());
}

void MeshGLWidget::drawPointsIfNeeded() {
  if (mesh_state.points_style == PointsStyle::kNone) return;

  glPointSize(mesh_state.point_size);

  if (mesh_state.points_style == PointsStyle::kCircle) {
    glEnable(GL_POINT_SMOOTH);
  }

  if (mesh_state.points_style == PointsStyle::kSquare) {
    glDisable(GL_POINT_SMOOTH);
  }

  program->setUniformValue("FragColor", mesh_state.points_color);
  glDrawArrays(GL_POINTS, 0, mesh.count_vertices);
}

void MeshGLWidget::drawLines() {
  if (mesh_state.lines_style == LinesStyle::kDashed) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x000f);
  }

  if (mesh_state.lines_style == LinesStyle::kSolid) {
    glDisable(GL_LINE_STIPPLE);
  }

  glLineWidth(mesh_state.lines_width);

  program->setUniformValue("FragColor", mesh_state.lines_color);

  element_buffer.bind();
  glDrawElements(GL_LINES, mesh_count_lines * 2, GL_UNSIGNED_INT, nullptr);
  element_buffer.release();
}

void MeshGLWidget::CalculateMVPMatrix() {
  GLfloat ratio = (double)this->width() / (double)this->height();

  mvp_matrix.setToIdentity();

  mvp_matrix.ortho(-1.0, 1.0, -1.0 / ratio, 1.0 / ratio, -1.0, 1.0);

  if (mesh_state.is_perspective) {
    mvp_matrix.perspective(90.0, 1, 0.0, 100.0);
    mvp_matrix.translate(0.0, 0.0, -2.0);
  }

  mvp_matrix.scale(mesh_state.scaling);
  mvp_matrix.translate(mesh_state.position);
  mvp_matrix.rotate(mesh_state.degree.x(), 1, 0, 0);
  mvp_matrix.rotate(mesh_state.degree.y(), 0, 1, 0);
  mvp_matrix.rotate(mesh_state.degree.z(), 0, 0, 1);
}

bool MeshGLWidget::loadObject(const QString& filename) {
  object_t object_to_load = {
      .vertices = NULL,
      .count_vertices = 0,
      .polygons = NULL,
      .count_polygons = 0,
  };

  QByteArray filename_bytes = filename.toLocal8Bit();
  int load_status = load_object(filename_bytes.data(), &object_to_load);
  bool is_loaded = (load_status == 0) ? true : false;

  if (is_loaded) {
    if (mesh_state.is_loaded) Cleanup();

    mesh = object_to_load;
    object_normalize(0.8, &mesh);
    initVertexBuffer();
    initElementBuffer();
    mesh_state.is_loaded = true;
  }

  update();
  return is_loaded;
}

void MeshGLWidget::Cleanup() {
  if (mesh_state.is_loaded == false) return;

  makeCurrent();

  vertex_array_object.destroy();
  vertex_buffer.destroy();
  element_buffer.destroy();

  mesh_count_lines = 0;
  mesh_state.is_loaded = false;

  free_object(&mesh);
}

void MeshGLWidget::moveObjectX(float x) {
  mesh_state.position.setX(x);
  update();
}

void MeshGLWidget::moveObjectY(float y) {
  mesh_state.position.setY(y);
  update();
}

void MeshGLWidget::moveObjectZ(float z) {
  mesh_state.position.setZ(z);
  update();
}

void MeshGLWidget::rotateObjectX(float x) {
  mesh_state.degree.setX(x);
  update();
}

void MeshGLWidget::rotateObjectY(float y) {
  mesh_state.degree.setY(y);
  update();
}

void MeshGLWidget::rotateObjectZ(float z) {
  mesh_state.degree.setZ(z);
  update();
}

void MeshGLWidget::scaleObject(float percent) {
  mesh_state.scaling = percent / 100.0;
  update();
}

void MeshGLWidget::setLineWidth(float width) {
  mesh_state.lines_width = width;
  update();
}

void MeshGLWidget::setLineColor(QColor& color) {
  mesh_state.lines_color = color;
  update();
}

void MeshGLWidget::setLineStyle(LinesStyle style) {
  mesh_state.lines_style = style;
  update();
}
void MeshGLWidget::setPointsColor(QColor& color) {
  mesh_state.points_color = color;
  update();
}

void MeshGLWidget::setPointsSize(float size) {
  mesh_state.point_size = size;
  update();
}

void MeshGLWidget::setPointsStyle(PointsStyle style) {
  mesh_state.points_style = style;
  update();
}

void MeshGLWidget::setBackground(QColor& color) {
  mesh_state.background = color;
  update();
}

void MeshGLWidget::setCentralPerspective(bool is_central_perspective) {
  mesh_state.is_perspective = is_central_perspective;
  update();
}

GLuint MeshGLWidget::getCountNodes() { return (GLuint)mesh.count_vertices; }

GLuint MeshGLWidget::getCountEdges() { return mesh_count_lines; }

}  // namespace ViewerFrontend
