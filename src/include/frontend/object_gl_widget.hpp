#ifndef FRONTEND_OBJECT_GL_WIDGET_H_
#define FRONTEND_OBJECT_GL_WIDGET_H_

#define GL_SILENCE_DEPRECATION

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "frontend/temp_cube_object.hpp"

extern "C" {
#include "backend/object_t.h"
}

namespace ViewerFrontend {

enum class PointsRenderType {
  kNone = 0,
  kCircle = 1,
  kSquare = 2,
};

enum class LinesRenderType {
  kSolid = 0,
  kDashed = 1,
};

class ObjectGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  ObjectGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

 private:
  object_t *object = &cube_object;
  double degree_step = 0.5;

  LinesRenderType lines_render_type = LinesRenderType::kDashed;
  vector_t lines_color = {1.0, 1.0, 1.0};
  double lines_width = 5.1;

  PointsRenderType points_render_type = PointsRenderType::kCircle;
  uint32_t points_size = 50;
  vector_t points_color = {1.0, 0.0, 0.0};

  GLuint vertices_vbo;

  void initializeGL() override;
  void paintGL() override;

  void loadVertex();

  void renderPointsIfNeeded();
  void renderLines();


 public slots:
  void redraw();
};

}  // namespace ViewerFrontend

#endif  // FRONTEND_OBJECT_GL_WIDGET_H_
