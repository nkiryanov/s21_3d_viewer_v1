#include "frontend/object_gl_widget.hpp"

extern "C" {
#include "backend/polygon_t.h"
#include "backend/transformations.h"
#include "backend/vector_t.h"
}

#include <cmath>

namespace ViewerFrontend {

void ObjectGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glEnableClientState(GL_VERTEX_ARRAY);

  loadObject();
}

void ObjectGLWidget::paintGL() {
  glLoadIdentity();
  glOrtho(-1.33, 1.33, -1, 1, -10, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  loadVerticesToBuffer();

  renderLines();
  renderPointsIfNeeded();
}

void ObjectGLWidget::loadObject() {
  if (object != nullptr) {
    free(object);
    deleteObjectBuffers();
  }

  object = &cube_object;
  object_normalize(0.5, object);

  generateObjectBuffers();
  loadPolygonsToBuffers();
}

void ObjectGLWidget::redraw() {
  static vector_t moving_deltas = {0.001, 0.001, 0.001};
  static vector_t to_move = {0.01, 0.01, 0.01};

  if (fabs(this->position.x) > 0.4) moving_deltas.x *= -1;
  if (fabs(this->position.y) > 0.3) moving_deltas.y *= -1;
  if (fabs(this->position.z) > 0.2) moving_deltas.z *= -1;

  to_move.x += moving_deltas.x;
  to_move.y += moving_deltas.y;
  to_move.z += moving_deltas.z;

  ObjectGLWidget::moveObjectX(to_move.x);
  ObjectGLWidget::moveObjectY(to_move.y);
  ObjectGLWidget::moveObjectZ(to_move.z);

  vector_t degree = {0.1, 0.1, 0.1};
  static vector_t degree_to_set = {0, 0, 0};
  degree_to_set.x += degree.x;
  degree_to_set.y += degree.y;
  degree_to_set.z += degree.z;

  ObjectGLWidget::rotateObjectX(degree_to_set.x);
  ObjectGLWidget::rotateObjectY(degree_to_set.y);
  ObjectGLWidget::rotateObjectZ(degree_to_set.z);

  static double scale = 2;
  static double scaling_step = -1;

  if (scale > 150 || scale < 0) scaling_step *= -1;

  scale += scaling_step;
  ObjectGLWidget::scaleObject(scale);

  ObjectGLWidget::setLineColor(255, 0, 255);
}

void ObjectGLWidget::generateObjectBuffers() {
  polygons_ebos.resize(object->polygons_amount);

  glGenBuffers(1, &vertices_vbo);
  glGenBuffers(object->polygons_amount, polygons_ebos.data());
}

void ObjectGLWidget::deleteObjectBuffers() {
  glGenBuffers(1, &vertices_vbo);
  glGenBuffers(polygons_ebos.size(), polygons_ebos.data());
}

void ObjectGLWidget::loadVerticesToBuffer() {
  // Load vertices to GPU and set vertex pointer to it
  vector_t *vertices = object->vertices;
  uint32_t vertices_size = sizeof(vector_t) * object->vertices_amount;

  glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);

  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);
  glVertexPointer(3, GL_DOUBLE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjectGLWidget::loadPolygonsToBuffers() {
  for (uint32_t i = 0; i != object->polygons_amount; ++i) {
    polygon_t polygon = object->polygons[i];

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygons_ebos[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * polygon.amount,
                 polygon.vertex_indexes, GL_STATIC_DRAW);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ObjectGLWidget::renderLines() {
  if (lines_style == LinesStyle::kDashed) {
    glLineStipple(1, 0x000f);
    glEnable(GL_LINE_STIPPLE);
  } else if (lines_style == LinesStyle::kSolid) {
    glDisable(GL_LINE_STIPPLE);
  }

  glLineWidth(lines_width);
  glColor3d(lines_color.x, lines_color.y, lines_color.z);

  //
  for (uint32_t i = 0; i != object->polygons_amount; ++i) {
    polygon_t polygon = object->polygons[i];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygons_ebos[i]);
    glDrawElements(GL_LINE_LOOP, polygon.amount, GL_UNSIGNED_INT, nullptr);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ObjectGLWidget::renderPointsIfNeeded() {
  if (points_style == PointsStyle::kNone) return;

  if (points_style == PointsStyle::kCircle) {
    glEnable(GL_POINT_SMOOTH);
  } else if (points_style == PointsStyle::kSquare) {
    glDisable(GL_POINT_SMOOTH);
  }

  glPointSize(points_size);
  glColor3d(points_color.x, points_color.y, points_color.z);

  glDrawArrays(GL_POINTS, 0, object->vertices_amount);
}

void ObjectGLWidget::moveObjectX(double x) {
  double x_to_move = x - this->position.x;
  this->position.x = x;
  object_move_x_axis(x_to_move, object);
  update();
}

void ObjectGLWidget::moveObjectY(double y) {
  double y_to_move = y - this->position.y;
  this->position.y = y;
  object_move_y_axis(y_to_move, object);
  update();
}

void ObjectGLWidget::moveObjectZ(double z) {
  double z_to_move = z - this->position.z;
  this->position.z = z;
  object_move_z_axis(z_to_move, object);
  update();
}

void ObjectGLWidget::rotateObjectX(double degree) {
  double degree_to_rotate = degree - this->degree.x;
  this->degree.x = degree;
  object_rotate_x_axis(degree_to_rotate, object);
  update();
}

void ObjectGLWidget::rotateObjectY(double degree) {
  double degree_to_rotate = degree - this->degree.y;
  this->degree.y = degree;
  object_rotate_y_axis(degree_to_rotate, object);
  update();
}

void ObjectGLWidget::rotateObjectZ(double degree) {
  double degree_to_rotate = degree - this->degree.z;
  this->degree.z = degree;
  object_rotate_z_axis(degree_to_rotate, object);
  update();
}

void ObjectGLWidget::scaleObject(double percent) {
  // Edge case. Do not try to scale for negative values
  if (percent <= 0) return;

  double relative_scale = percent / this->scaling;
  this->scaling = percent;
  object_scale(relative_scale, object);
  update();
}

void ObjectGLWidget::setLineWidth(double width) {
  lines_width = width;
  update();
}

void ObjectGLWidget::setLineColor(int r, int g, int b) {
  this->lines_color = {r / 255.0, g / 255.0, b / 255.0};
  update();
}

void ObjectGLWidget::setLineStyle(LinesStyle style) {
  this->lines_style = style;
  update();
}

void ObjectGLWidget::setPointsColor(int r, int g, int b) {
  this->points_color = {r / 255.0, g / 255.0, b / 255.0};
  update();
}

void ObjectGLWidget::setPointsSize(double size) {
  this->points_size = size;
  update();
}

void ObjectGLWidget::setPointsStyle(PointsStyle style) {
  this->points_style = style;
  update();
}

}  // namespace ViewerFrontend
