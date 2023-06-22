#ifndef FRONTEND_MODEL_VIEWER_H_
#define FRONTEND_MODEL_VIEWER_H_

#include <QMainWindow>
#include <QTimer>

#include "frontend/object_gl_widget.hpp"

QT_BEGIN_NAMESPACE namespace Ui { class ModelViewer; }
QT_END_NAMESPACE

namespace ViewerFrontend {

class ModelViewer : public QMainWindow {
  Q_OBJECT

 public:
  ModelViewer(QWidget *parent = nullptr);
  ~ModelViewer();

 private slots:
  void on_pushButton_build_clicked();

  void on_zoom_slider_valueChanged(int value);

  void on_zoom_nodes_slider_valueChanged(int value);
  void on_pushButton_set_nodes_color_clicked();
  void on_pushButton_set_edges_color_clicked();
  void on_edges_width_spinBox_valueChanged(int arg1);

  void on_move_scroll_bar_x_valueChanged(int value);
  void on_move_scroll_bar_y_valueChanged(int value);
  void on_move_scroll_bar_z_valueChanged(int value);

  void on_rotate_scroll_bar_x_valueChanged(int value);
  void on_rotate_scroll_bar_y_valueChanged(int value);
  void on_rotate_scroll_bar_z_valueChanged(int value);

  void on_actionOpen_triggered();
  void on_actionBackground_Color_triggered();
  void on_actionSave_picture_triggered();

  void on_sizeNodes_radioButton_none_clicked();
  void on_sizeNodes_radioButton_square_clicked();
  void on_sizeNodes_radioButton_circle_clicked();
  void on_edges_solid_radioButton_clicked();
  void on_edges_dashed_radioButton_clicked();

 private:
  Ui::ModelViewer *ui;
  ObjectGLWidget *object_widget;
};

}  // namespace ViewerFrontend

#endif  // FRONTEND_MODEL_VIEWER_H_
