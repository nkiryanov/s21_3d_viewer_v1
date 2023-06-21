#include "frontend/object_gl_widget.hpp"

extern "C" {
#include "backend/polygon_t.h"
#include "backend/transformations.h"
#include "backend/vector_t.h"
}

#include <cmath>

namespace ViewerFrontend {

void ObjectGLWidget::initializeGL() {
  object_normalize(0.5, object);

  initializeOpenGLFunctions();

  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();
  glOrtho(-1.33, 1.33, -1, 1, -10, 1);
}

void ObjectGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  loadVertex();

  glEnableClientState(GL_VERTEX_ARRAY);

  renderPointsIfNeeded();
  renderLines();

}

void ObjectGLWidget::redraw() {
  object_rotate_x_axis(degree_step, object);
  object_rotate_y_axis(degree_step, object);
  object_rotate_z_axis(degree_step, object);

  update();
}

void ObjectGLWidget::loadVertex() {
  // Load vertices to GPU and set vertex pointer to it
  uint32_t size = sizeof(vector_t) * object->vertices_amount;

  glGenBuffers(1, &vertices_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);

  glBufferData(GL_ARRAY_BUFFER, size, object->vertices, GL_STREAM_DRAW);
  glVertexPointer(3, GL_DOUBLE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjectGLWidget::renderPointsIfNeeded() {
  if (points_render_type == PointsRenderType::kNone) return;

  if (points_render_type == PointsRenderType::kCircle) {
    glEnable(GL_POINT_SMOOTH);
  } else if (points_render_type == PointsRenderType::kSquare) {
    glDisable(GL_POINT_SMOOTH);
  }

  glPointSize(points_size);
  glColor3d(points_color.x, points_color.y, points_color.z);

  glDrawArrays(GL_POINTS, 0, object->vertices_amount);
}

void ObjectGLWidget::renderLines() {
  if (lines_render_type == LinesRenderType::kDashed) {
    glLineStipple(1, 0x000f);
    glEnable(GL_LINE_STIPPLE);
  } else if (lines_render_type == LinesRenderType::kSolid) {
    glDisable(GL_LINE_STIPPLE);
  }

  glLineWidth(lines_width);
  glColor3d(lines_color.x, lines_color.y, lines_color.z);

  for (uint32_t i = 0; i != object->polygons_amount; ++i) {
    polygon_t polygon = object->polygons[i];
    glDrawElements(GL_LINE_LOOP, polygon.amount, GL_UNSIGNED_INT,
                   polygon.vertex_indexes);
  }
}

}  // namespace ViewerFrontend
