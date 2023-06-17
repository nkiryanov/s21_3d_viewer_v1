#ifndef FRONTEND_MODEL_VIEWER_H_
#define FRONTEND_MODEL_VIEWER_H_

#include <QMainWindow>

#include "frontend/object_gl_widget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class ModelViewer;
}
QT_END_NAMESPACE

namespace ViewerFrontend {

class ModelViewer : public QMainWindow {
  Q_OBJECT

 public:
  ModelViewer(QWidget *parent = nullptr);
  ~ModelViewer();

 private:
  Ui::ModelViewer *ui;
  ObjectGLWidget *object_widget;
};

}  // namespace ViewerFrontend

#endif  // FRONTEND_MODEL_VIEWER_H_
