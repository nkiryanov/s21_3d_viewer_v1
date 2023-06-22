#include "frontend/model_viewer.hpp"

#include <QColorDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QResizeEvent>
#include <QtGlobal>
// 3rd bonus task
#include <QFileDialog>

#include "frontend/object_gl_widget.hpp"
#include "ui/ui_model_viewer.h"

extern "C" {
#include "backend/polygon_t.h"
#include "backend/transformations.h"
#include "backend/vector_t.h"
}

inline void initModelViewerResources() { Q_INIT_RESOURCE(resources); }

void saveImageToFile(const QImage &image, const QString &filePath) {
  image.save(filePath);
}

namespace ViewerFrontend {

ModelViewer::ModelViewer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ModelViewer) {
  initModelViewerResources();  // add resources

  ui->setupUi(this);

  QLabel *buildStatusLabel = ui->build_status_label;
  QLabel *nodesValueLabel = ui->nodes_value_label;
  QLabel *edgesValueLabel = ui->edges_value_label;
  QLabel *fileNameLabel = ui->file_name_label;

  QStatusBar *statusBar = QMainWindow::statusBar();  // get pointer to statusBar
  statusBar->addWidget(buildStatusLabel);
  statusBar->addPermanentWidget(fileNameLabel);
  statusBar->addPermanentWidget(nodesValueLabel);
  statusBar->addPermanentWidget(edgesValueLabel);

  QPalette palette =
      ui->color_nodes_widget->palette();  // set background color on palette
                                          // widgets (before choosing color)
  palette.setColor(QPalette::Window, "white");
  ui->color_nodes_widget->setPalette(palette);
  ui->color_nodes_widget->setAutoFillBackground(true);

  ui->color_edges_widget->setPalette(palette);
  ui->color_edges_widget->setAutoFillBackground(true);
}

ModelViewer::~ModelViewer() { delete ui; }

// top menu bar start

void ModelViewer::on_actionOpen_triggered() {
  QString fileName = QFileDialog::getOpenFileName(this, "Choose .obj...", ".",
                                                  "OBJ File (*.obj)");
  QFileInfo fileInfo(fileName);
  QString baseName = fileInfo.fileName();
  ui->file_name_label->setText(baseName);
}

void ModelViewer::on_actionBackground_Color_triggered() {
  QColorDialog bg_dialog;
  QColor color = bg_dialog.getColor();
  if (color.isValid()) {
    int red, green, blue = 0;
    color.getRgb(&red, &green, &blue);
    ui->ObjectGLWidget->setBackground(red, green, blue);
  }
}

void ModelViewer::on_actionSave_picture_triggered() {
  {
    QPixmap screenshot =
        ui->ObjectGLWidget->grab();  // Grab a screenshot of the ObjectGLWidget
    QString filePath = QFileDialog::getSaveFileName(
        this, "Save Image", ".",
        "BMP Files (*.bmp);;JPEG Files (*.jpg)");  // Link to save image
    if (!filePath.isEmpty()) {                     // Save image
      screenshot.save(filePath);
    }
  }
}

// top menu bar end

void ModelViewer::on_pushButton_build_clicked() {
  QLabel *nodesValueLabel = ui->nodes_value_label;
  QLabel *edgesValueLabel = ui->edges_value_label;
  QLabel *buildStatusLabel = ui->build_status_label;
  int edges = 6;  // need funcs for count this
  int nodes = 8;
  nodesValueLabel->setText("Nodes: " + QString::number(nodes));
  edgesValueLabel->setText("Edges: " + QString::number(edges));
  ui->build_status_label->setStyleSheet(
      "color: green");                            // if success then else
  buildStatusLabel->setText(QString("Success"));  // "ERROR red"
  QLabel *buildStatusPicLabel = ui->build_status_pic_label;
  QPixmap pixmap(":/images/success.png");
  int width_status_image = ui->build_status_pic_label->width();
  int height_status_image = ui->build_status_pic_label->height();
  statusBar()->addPermanentWidget(nodesValueLabel);
  statusBar()->addPermanentWidget(edgesValueLabel);
  statusBar()->addWidget(buildStatusPicLabel);
  buildStatusPicLabel->setPixmap(pixmap.scaled(
      width_status_image, height_status_image, Qt::KeepAspectRatio));
}

void ModelViewer::on_zoom_slider_valueChanged(int value) {
  ui->zoom_value_label->setText(QString::number(value) + "%");
  ui->ObjectGLWidget->scaleObject(value);
}

void ModelViewer::on_zoom_nodes_slider_valueChanged(int value) {
  ui->zoom_nodes_value_label->setText(QString::number(value) + "%");
  ui->ObjectGLWidget->setPointsSize(value / 2.0);
}

void ModelViewer::on_pushButton_set_nodes_color_clicked() {
  QColorDialog nodes_dialog;
  QColor color = nodes_dialog.getColor();

  if (color.isValid()) {
    QPalette palette = ui->color_nodes_widget->palette();
    palette.setColor(QPalette::Window, color);
    ui->color_nodes_widget->setPalette(palette);
    ui->color_nodes_widget->setAutoFillBackground(true);

    int red, green, blue = 0;
    color.getRgb(&red, &green, &blue);
    ui->ObjectGLWidget->setPointsColor(red, green, blue);
  }
}

void ModelViewer::on_pushButton_set_edges_color_clicked() {
  QColorDialog edges_dialog;
  QColor color = edges_dialog.getColor();

  if (color.isValid()) {
    QPalette palette = ui->color_edges_widget->palette();
    palette.setColor(QPalette::Window, color);
    ui->color_edges_widget->setPalette(palette);
    ui->color_edges_widget->setAutoFillBackground(true);

    int red, green, blue = 0;
    color.getRgb(&red, &green, &blue);
    ui->ObjectGLWidget->setLineColor(red, green, blue);
  }
}

void ModelViewer::on_edges_width_spinBox_valueChanged(int value) {
  ui->ObjectGLWidget->setLineWidth(value);
}

void ModelViewer::on_move_scroll_bar_x_valueChanged(int value) {
  float position = static_cast<float>(value / 200.0);
  ui->label_move_value_x->setText(QString::number(position, 'f', 2));
  ui->ObjectGLWidget->moveObjectX(position);
}

void ModelViewer::on_move_scroll_bar_y_valueChanged(int value) {
  float position = static_cast<float>(value / 200.0);
  ui->label_move_value_y->setText(QString::number(position, 'f', 2));
  ui->ObjectGLWidget->moveObjectY(position);
}

void ModelViewer::on_move_scroll_bar_z_valueChanged(int value) {
  float position = static_cast<float>(value / 200.0);
  ui->label_move_value_z->setText(QString::number(position, 'f', 2));
  ui->ObjectGLWidget->moveObjectZ(position);
}

void ModelViewer::on_rotate_scroll_bar_x_valueChanged(int value) {
  ui->label_rotate_value_x->setText(QString::number(value) + "°");
  ui->ObjectGLWidget->rotateObjectX(value);
}

void ModelViewer::on_rotate_scroll_bar_y_valueChanged(int value) {
  ui->label_rotate_value_y->setText(QString::number(value) + "°");
  ui->ObjectGLWidget->rotateObjectY(value);
}

void ModelViewer::on_rotate_scroll_bar_z_valueChanged(int value) {
  ui->label_rotate_value_z->setText(QString::number(value) + "°");
  ui->ObjectGLWidget->rotateObjectZ(value);
}

void ModelViewer::on_sizeNodes_radioButton_none_clicked() {
  ui->ObjectGLWidget->setPointsStyle(PointsStyle::kNone);
}

void ModelViewer::on_sizeNodes_radioButton_square_clicked() {
  ui->ObjectGLWidget->setPointsStyle(PointsStyle::kSquare);
}

void ModelViewer::on_sizeNodes_radioButton_circle_clicked() {
  ui->ObjectGLWidget->setPointsStyle(PointsStyle::kCircle);
}

void ModelViewer::on_edges_solid_radioButton_clicked() {
  ui->ObjectGLWidget->setLineStyle(LinesStyle::kSolid);
}

void ModelViewer::on_edges_dashed_radioButton_clicked() {
  ui->ObjectGLWidget->setLineStyle(LinesStyle::kDashed);
}

}  // namespace ViewerFrontend
