#ifndef FRONTEND_OBJECT_GL_WIDGET_H_
#define FRONTEND_OBJECT_GL_WIDGET_H_

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QVector3D>
#include <vector>

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

struct MeshState {
  bool is_loaded = false;
  bool is_perspective = true;

  GLfloat point_size = 10.0;
  PointsStyle points_style = PointsStyle::kNone;
  QColor points_color = QColor("white");

  GLfloat lines_width = 1.0;
  LinesStyle lines_style = LinesStyle::kSolid;
  QColor lines_color = QColor("white");

  QColor background = QColor("black");
  QVector3D degree;
  QVector3D position;
  GLfloat scaling = 1.0;
};

class MeshGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  MeshGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}
  ~MeshGLWidget();

 protected:
  void initializeGL() override;
  void paintGL() override;

 private:
  QOpenGLShaderProgram *program = nullptr;
  QOpenGLBuffer vertex_buffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  QOpenGLBuffer element_buffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
  QOpenGLVertexArrayObject vertex_array_object;

  QMatrix4x4 mvp_matrix;

  object_t mesh;
  MeshState mesh_state;
  GLuint mesh_edges_count;

  void initShaders();
  void initVertexBuffer();
  void destroyVertexBuffer();
  void initElementBuffer();
  void destroyElementBuffer();

  void CalculateMVPMatrix();

  void drawBackground();
  void drawPointsIfNeeded();
  void drawLines();

 public slots:
  bool loadObject(const QString &filename);
  void Cleanup();
  void moveObjectX(float x);
  void moveObjectY(float y);
  void moveObjectZ(float z);
  void rotateObjectX(float x);
  void rotateObjectY(float y);
  void rotateObjectZ(float z);
  void scaleObject(float percent);
  void setLineWidth(float width);
  void setLineColor(QColor &color);
  void setLineStyle(LinesStyle style);
  void setPointsColor(QColor &color);
  void setPointsSize(float size);
  void setPointsStyle(PointsStyle style);
  void setBackground(QColor &color);
  void setCentralPerspective(bool is_central_perspective);
};

}  // namespace ViewerFrontend

#endif  // FRONTEND_OBJECT_GL_WIDGET_H_
