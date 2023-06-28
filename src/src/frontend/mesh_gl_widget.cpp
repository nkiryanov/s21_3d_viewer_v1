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

void MeshGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  initShaders();
  drawBackground();
}

void MeshGLWidget::paintGL() {
  drawBackground();

  // Edge case. Do not draw the object if it not loaded
  if (mesh_state.is_loaded == false) return;

  CalculateMVPMatrix();

  program.bind();
  program.setUniformValue("mvp_matrix", mvp_matrix);

  vertex_array_object.bind();
  drawPointsIfNeeded();
  drawLines();
  vertex_array_object.release();

  program.release();
}

void MeshGLWidget::initShaders() {
  // Compile vertex shader
  if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                       ":/shaders/vertex.glsl"))
    close();

  // Compile fragment shader
  if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                       ":/shaders/fragment.glsl"))
    close();

  // Link shader pipeline
  if (!program.link()) close();

  // Bind shader pipeline for use
  if (!program.bind()) close();
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

void MeshGLWidget::destroyVertexBuffer() {
  vertex_buffer.destroy();
  vertex_array_object.destroy();
}

void MeshGLWidget::initElementBuffer() {
  // Store all the polygons in one buffer element array.
  // Cause the number of polygons may vary we should draw the polygons with
  // `glMultiDrawElements` and have to store count indices per polygons and
  // calculate the offset in the buffer.

  std::vector<GLuint> polygon_indices;
  GLuint buffer_offset = 0;

  for (uint32_t i = 0; i != mesh.count_polygons; ++i) {
    auto& polygon = mesh.polygons[i];

    element_indices_counts.push_back(polygon.count_indices);
    element_offsets.push_back((void*)(size_t)buffer_offset);

    buffer_offset += polygon.count_indices * sizeof(GLuint);

    for (uint32_t j = 0; j != polygon.count_indices; ++j) {
      polygon_indices.push_back((GLuint)polygon.vertex_indices[j]);
    }
  }

  element_buffer.create();
  element_buffer.bind();
  element_buffer.allocate(polygon_indices.data(),
                          polygon_indices.size() * sizeof(GLuint));
  element_buffer.release();
}

void MeshGLWidget::destroyElementBuffer() { element_buffer.destroy(); }

void MeshGLWidget::drawBackground() {
  glClearColor(mesh_state.background.redF(), mesh_state.background.greenF(),
               mesh_state.background.blueF(), mesh_state.background.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_TEST);
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

  program.setUniformValue("FragColor", mesh_state.points_color);
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

  program.setUniformValue("FragColor", mesh_state.lines_color);

  element_buffer.bind();
  glMultiDrawElements(GL_LINES, element_indices_counts.data(), GL_UNSIGNED_INT,
                      element_offsets.data(), element_offsets.size());
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

int MeshGLWidget::loadObject(const QString& filename) {
  if (mesh_state.is_loaded) {
    free_object(&mesh);
    destroyVertexBuffer();
    destroyElementBuffer();
    element_indices_counts.clear();
    element_offsets.clear();

    mesh_state.is_loaded = false;
  }

  QByteArray filename_bytes = filename.toLocal8Bit();
  int load_status = load_object(filename_bytes.data(), &mesh);

  bool is_error_happened = (load_status == 0) ? false : true;

  if (!is_error_happened) {
    object_normalize(0.8, &mesh);
    initVertexBuffer();
    initElementBuffer();
    mesh_state.is_loaded = true;
  }

  return is_error_happened;
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

}  // namespace ViewerFrontend
