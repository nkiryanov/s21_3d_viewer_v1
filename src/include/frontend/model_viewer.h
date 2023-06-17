#ifndef FRONTEND_MODEL_VIEWER_H_
#define FRONTEND_MODEL_VIEWER_H_

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class ModelViewer;
}
QT_END_NAMESPACE

class ModelViewer : public QMainWindow {
  Q_OBJECT

 public:
  ModelViewer(QWidget *parent = nullptr);
  ~ModelViewer();

 private:
  Ui::ModelViewer *ui;
};
#endif  // FRONTEND_MODEL_VIEWER_H_
