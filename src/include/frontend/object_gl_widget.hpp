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

enum class PointsStyle {
  kNone = 0,
  kCircle = 1,
  kSquare = 2,
};

enum class LinesStyle {
  kSolid = 0,
  kDashed = 1,
};

class ObjectGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  ObjectGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}
  void loadObject();

 private:
  object_t *object = nullptr;
  GLuint vertices_vbo;
  std::vector<GLuint> polygons_ebos;

  LinesStyle lines_style = LinesStyle::kSolid;
  vector_t lines_color = {1.0, 1.0, 1.0};
  double lines_width = 0.1;

  PointsStyle points_style = PointsStyle::kNone;
  uint32_t points_size = 10;
  vector_t points_color = {1.0, 1.0, 1.0};

  vector_t degree = {0.0, 0.0, 0.0};
  vector_t position = {0.0, 0.0, 0.0};

  double scaling = 100;

  void initializeGL() override;
  void paintGL() override;

  void generateObjectBuffers();
  void deleteObjectBuffers();
  void loadPolygonsToBuffers();
  void loadVerticesToBuffer();

  void renderLines();
  void renderPointsIfNeeded();

 public slots:
  void moveObjectX(double x);
  void moveObjectY(double y);
  void moveObjectZ(double z);

  void rotateObjectX(double degree);
  void rotateObjectY(double degree);
  void rotateObjectZ(double degree);

  void scaleObject(double percent);

  void setLineWidth(double width);
  void setLineColor(int r, int g, int b);
  void setLineStyle(LinesStyle style);

  void setPointsColor(int r, int g, int b);
  void setPointsSize(double size);
  void setPointsStyle(PointsStyle style);

  void redraw();
};

}  // namespace ViewerFrontend

#endif  // FRONTEND_OBJECT_GL_WIDGET_H_
