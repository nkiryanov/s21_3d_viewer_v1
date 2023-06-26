#include "frontend/mesh_gl_widget.hpp"

extern "C" {
#include "backend/polygon_t.h"
#include "backend/vector_t.h"
}

#include <QDebug>
#include <cmath>
#include <unordered_set>

namespace ViewerFrontend {

void MeshGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  initShaders();
  initVertexBuffer();
  initElementBuffers();

  drawBackground();
}

void MeshGLWidget::paintGL() {
  drawBackground();

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
  vertex_buffer.allocate(mesh->vertices,
                         sizeof(vector_t) * mesh->vertices_amount);
  vertex_buffer.release();

  vertex_array_object.create();
  vertex_array_object.bind();

  vertex_buffer.bind();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);
  vertex_buffer.release();

  vertex_array_object.release();
}

void MeshGLWidget::initElementBuffers() {
  // Aggregate same count indices polygons in the same storage and
  // buffer to GPU

  // The temp structure to store glBufferNumber and polygons indexes that
  // is awaiting to upload to GPU.
  // It's needed to accumulate polygons with same indexes count.
  struct DataToUpload {
    GLuint buffer_number;
    std::vector<GLuint> vertex_indices;
  };

  std::unordered_map<uint32_t, DataToUpload> data_per_buffer;

  for (uint32_t i = 0; i != mesh->polygons_amount; ++i) {
    polygon_t polygon = mesh->polygons[i];

    auto& data = data_per_buffer[polygon.amount];

    // Generate EBO if buffer empty
    if (data.vertex_indices.size() == 0) glGenBuffers(1, &data.buffer_number);

    // Combine polygons with same indexes count to same buffer_data
    for (uint32_t i = 0; i != polygon.amount; ++i) {
      data.vertex_indices.push_back((GLuint)polygon.vertex_indexes[i]);
    }
  }

  for (const auto& [_, data] : data_per_buffer) {
    ElementBuffer element_buffer;

    element_buffer.buffer_number = data.buffer_number;
    element_buffer.buffer_size = data.vertex_indices.size() * sizeof(GLuint);

    element_buffers.push_back(element_buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer.buffer_number);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer.buffer_size,
                 data.vertex_indices.data(), GL_STATIC_DRAW);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

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
  glDrawArrays(GL_POINTS, 0, mesh->vertices_amount);
}

void MeshGLWidget::drawLines() {
  if (mesh_state.lines_style == LinesStyle::kDashed) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x000f);
  }

  glLineWidth(mesh_state.lines_width);

  program.setUniformValue("FragColor", mesh_state.lines_color);

  for (const auto& element_buffer : element_buffers) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer.buffer_number);
    glDrawElements(GL_LINE_LOOP, element_buffer.buffer_size, GL_UNSIGNED_INT,
                   nullptr);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

}  // namespace ViewerFrontend
