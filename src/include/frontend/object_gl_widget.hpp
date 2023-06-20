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

class ObjectGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  ObjectGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

 private:
  object_t *object = &cube_object;
  double degree_step = 0.5;

  void initializeGL() override;
  void paintGL() override;

 public slots:
  void redraw();
};

}  // namespace ViewerFrontend

#endif  // FRONTEND_OBJECT_GL_WIDGET_H_
