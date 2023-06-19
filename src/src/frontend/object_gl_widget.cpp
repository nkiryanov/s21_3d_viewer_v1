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

  normalize_vertices(0.3, object);
}

void ObjectGLWidget::paintGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (uint32_t i = 0; i != object->polygons_amount; ++i) {
    polygon_t polygon = object->polygons[i];
    glBegin(GL_LINE_LOOP);

    for (uint32_t j = 0; j != polygon.amount; ++j) {
      uint32_t vertex_index = polygon.vertex_indexes[j];
      vector_t vertex = object->vertices[vertex_index];

      glVertex3d(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
  }
}

}  // namespace ViewerFrontend
