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
  glOrtho(-1.33, 1.33, -1, 1, -10, 10);
}

void ObjectGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float color_part = 0.1;

  for (uint32_t i = 0; i != object->polygons_amount; ++i) {
    polygon_t polygon = object->polygons[i];
    glBegin(GL_LINE_LOOP);

    glColor3f(1.0, color_part, color_part);

    for (uint32_t j = 0; j != polygon.amount; ++j) {
      uint32_t vertex_index = polygon.vertex_indexes[j];
      vector_t vertex = object->vertices[vertex_index];

      glVertex3d(vertex.x, vertex.y, vertex.z);
    }

    color_part = fmod(color_part + 0.1, 100);

    glEnd();
  }
}

void ObjectGLWidget::redraw() {
  object_rotate_x_axis(degree_step, object);
  object_rotate_y_axis(degree_step, object);
  object_rotate_z_axis(degree_step, object);

  update();
}

}  // namespace ViewerFrontend
