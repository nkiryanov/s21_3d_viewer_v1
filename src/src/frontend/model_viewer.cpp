#include "frontend/model_viewer.hpp"

#include <QColorDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QResizeEvent>
#include <QtGlobal>
// 3rd bonus task
#include <QFileDialog>
#include <QImage>
#include <QImageWriter>
#include <QSettings>
#include <QTimer>
#include <QVector>
#include <cstdint>
#include <vector>

#include "frontend/gif.h"
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

  // set background color on palette widgets (before choosing color)
  QPalette palette = ui->color_nodes_widget->palette();

  palette.setColor(QPalette::Window, "white");
  ui->color_nodes_widget->setPalette(palette);
  ui->color_nodes_widget->setAutoFillBackground(true);

  ui->color_edges_widget->setPalette(palette);
  ui->color_edges_widget->setAutoFillBackground(true);

  palette.setColor(QPalette::Window, "black");
  ui->color_bg_widget->setPalette(palette);
  ui->color_bg_widget->setAutoFillBackground(true);

  loadSettings();
}

ModelViewer::~ModelViewer() {
  saveSettings();
  delete ui;
}

// top menu bar start

void ModelViewer::on_actionOpen_triggered() {
  QString fileName = QFileDialog::getOpenFileName(this, "Choose .obj...", ".",
                                                  "OBJ File (*.obj)");
  QFileInfo fileInfo(fileName);
  QString baseName = fileInfo.fileName();
  ui->file_name_label->setText(baseName);
}

void ModelViewer::on_actionSave_picture_triggered() {
  // Grab a screenshot of the MeshGLWidget
  QPixmap screenshot = ui->MeshGLWidget->grab();

  // Link to save image
  QString filePath = QFileDialog::getSaveFileName(
      this, "Save Image", ".", "BMP Files (*.bmp);;JPEG Files (*.jpg)");

  // Save image
  if (!filePath.isEmpty()) {
    screenshot.save(filePath);
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
  ui->MeshGLWidget->scaleObject(value);
}

void ModelViewer::on_zoom_nodes_slider_valueChanged(int value) {
  ui->zoom_nodes_value_label->setText(QString::number(value) + "%");
  ui->MeshGLWidget->setPointsSize(value / 2.0);
}

void ModelViewer::on_pushButton_set_bg_color_clicked() {
  QColorDialog bg_dialog;
  QColor color = bg_dialog.getColor();
  if (color.isValid()) {
    QPalette palette = ui->color_bg_widget->palette();
    palette.setColor(QPalette::Window, color);
    ui->color_bg_widget->setPalette(palette);
    ui->color_bg_widget->setAutoFillBackground(true);

    ui->MeshGLWidget->setBackground(color);
  }
}

void ModelViewer::on_pushButton_set_nodes_color_clicked() {
  QColorDialog nodes_dialog;
  QColor color = nodes_dialog.getColor();

  if (color.isValid()) {
    QPalette palette = ui->color_nodes_widget->palette();
    palette.setColor(QPalette::Window, color);
    ui->color_nodes_widget->setPalette(palette);
    ui->color_nodes_widget->setAutoFillBackground(true);

    ui->MeshGLWidget->setPointsColor(color);
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

    ui->MeshGLWidget->setLineColor(color);
  }
}

void ModelViewer::on_edges_width_spinBox_valueChanged(int value) {
  ui->MeshGLWidget->setLineWidth(value);
}

void ModelViewer::on_move_scroll_bar_x_valueChanged(int value) {
  float position = static_cast<float>(value / 20.0);
  ui->label_move_value_x->setText(QString::number(position, 'f', 2));
  ui->MeshGLWidget->moveObjectX(position);
}

void ModelViewer::on_move_scroll_bar_y_valueChanged(int value) {
  float position = static_cast<float>(value / 20.0);
  ui->label_move_value_y->setText(QString::number(position, 'f', 2));
  ui->MeshGLWidget->moveObjectY(position);
}

void ModelViewer::on_move_scroll_bar_z_valueChanged(int value) {
  float position = static_cast<float>(value / 20.0);
  ui->label_move_value_z->setText(QString::number(position, 'f', 2));
  ui->MeshGLWidget->moveObjectZ(position);
}

void ModelViewer::on_rotate_scroll_bar_x_valueChanged(int value) {
  ui->label_rotate_value_x->setText(QString::number(value) + "°");
  ui->MeshGLWidget->rotateObjectX(value);
}

void ModelViewer::on_rotate_scroll_bar_y_valueChanged(int value) {
  ui->label_rotate_value_y->setText(QString::number(value) + "°");
  ui->MeshGLWidget->rotateObjectY(value);
}

void ModelViewer::on_rotate_scroll_bar_z_valueChanged(int value) {
  ui->label_rotate_value_z->setText(QString::number(value) + "°");
  ui->MeshGLWidget->rotateObjectZ(value);
}

void ModelViewer::on_sizeNodes_radioButton_none_clicked() {
  ui->MeshGLWidget->setPointsStyle(PointsStyle::kNone);
}

void ModelViewer::on_sizeNodes_radioButton_square_clicked() {
  ui->MeshGLWidget->setPointsStyle(PointsStyle::kSquare);
}

void ModelViewer::on_sizeNodes_radioButton_circle_clicked() {
  ui->MeshGLWidget->setPointsStyle(PointsStyle::kCircle);
}

void ModelViewer::on_edges_solid_radioButton_clicked() {
  ui->MeshGLWidget->setLineStyle(LinesStyle::kSolid);
}

void ModelViewer::on_edges_dashed_radioButton_clicked() {
  ui->MeshGLWidget->setLineStyle(LinesStyle::kDashed);
}

void ModelViewer::saveSettings() {
  QSettings settings("3D_Viewer_Settings", "3d_viewer", this);

  int lineStyle = ui->edges_solid_radioButton->isChecked()
                      ? 0
                      : 1;  // 0 - сплошная, 1 - пунктирная
  settings.setValue("lineStyle", lineStyle);

  int edgesWidth = ui->edges_width_spinBox->value();
  settings.setValue("edgesWidth", edgesWidth);

  int nodeStyle = 0;
  if (ui->sizeNodes_radioButton_square->isChecked()) {
    nodeStyle = 1;
  } else if (ui->sizeNodes_radioButton_circle->isChecked()) {
    nodeStyle = 2;
  }
  settings.setValue("nodeStyle", nodeStyle);

  int nodeSize = ui->zoom_nodes_slider->value();
  settings.setValue("nodeSize", nodeSize);

  QColor bgColor = ui->color_bg_widget->palette().color(QPalette::Window);
  int bgRed = bgColor.red();
  int bgGreen = bgColor.green();
  int bgBlue = bgColor.blue();
  settings.setValue("bgRed", bgRed);
  settings.setValue("bgGreen", bgGreen);
  settings.setValue("bgBlue", bgBlue);

  QColor edgesColor = ui->color_edges_widget->palette().color(QPalette::Window);
  int edgesRed = edgesColor.red();
  int edgesGreen = edgesColor.green();
  int edgesBlue = edgesColor.blue();
  settings.setValue("edgesRed", edgesRed);
  settings.setValue("edgesGreen", edgesGreen);
  settings.setValue("edgesBlue", edgesBlue);

  QColor nodesColor = ui->color_nodes_widget->palette().color(QPalette::Window);
  int nodesRed = nodesColor.red();
  int nodesGreen = nodesColor.green();
  int nodesBlue = nodesColor.blue();
  settings.setValue("nodesRed", nodesRed);
  settings.setValue("nodesGreen", nodesGreen);
  settings.setValue("nodesBlue", nodesBlue);
}

void ModelViewer::loadSettings() {
  QSettings settings("3D_Viewer_Settings", "3d_viewer", this);
  QColor loaded_color = QColor();

  // Load lines style. If not set use solid lines
  int lineStyle = settings.value("lineStyle", 0).toInt();
  if (lineStyle == 1) {
    ui->MeshGLWidget->setLineStyle(LinesStyle::kDashed);
    ui->edges_dashed_radioButton->setChecked(lineStyle == 1);
  } else {
    ui->MeshGLWidget->setLineStyle(LinesStyle::kSolid);
    ui->edges_solid_radioButton->setChecked(lineStyle == 0);
  }

  int edgesWidth = settings.value("edgesWidth", 1).toInt();
  ui->edges_width_spinBox->setValue(edgesWidth);
  ui->MeshGLWidget->setLineWidth(edgesWidth);

  int nodeStyle = settings.value("nodeStyle", 0).toInt();
  if (nodeStyle == 0) {
    ui->MeshGLWidget->setPointsStyle(PointsStyle::kNone);
    ui->sizeNodes_radioButton_none->setChecked(nodeStyle == 0);
  } else if (nodeStyle == 2) {
    ui->MeshGLWidget->setPointsStyle(PointsStyle::kCircle);
    ui->sizeNodes_radioButton_circle->setChecked(nodeStyle == 2);
  } else {
    ui->MeshGLWidget->setPointsStyle(PointsStyle::kSquare);
    ui->sizeNodes_radioButton_square->setChecked(nodeStyle == 1);
  }

  int nodeSize = settings.value("nodeSize", 1).toInt();
  ui->MeshGLWidget->setPointsSize(nodeSize / 2.0);
  ui->zoom_nodes_value_label->setText(QString::number(nodeSize) + "%");
  ui->zoom_nodes_slider->setValue(nodeSize);

  // Load background color
  loaded_color = QColor(settings.value("bgRed", 0).toInt(),
                        settings.value("bgGreen", 0).toInt(),
                        settings.value("bgBlue", 0).toInt());
  ui->MeshGLWidget->setBackground(loaded_color);
  QPalette palette;
  palette.setColor(QPalette::Window, loaded_color);
  ui->color_bg_widget->setPalette(palette);
  ui->color_bg_widget->setAutoFillBackground(true);

  // Load edges color
  loaded_color = QColor(settings.value("edgesRed", 0).toInt(),
                        settings.value("edgesGreen", 0).toInt(),
                        settings.value("edgesBlue", 0).toInt());
  ui->MeshGLWidget->setLineColor(loaded_color);
  palette.setColor(QPalette::Window, loaded_color);
  ui->color_edges_widget->setPalette(palette);
  ui->color_edges_widget->setAutoFillBackground(true);

  // Load edges color
  loaded_color = QColor(settings.value("nodesRed", 0).toInt(),
                        settings.value("nodesGreen", 0).toInt(),
                        settings.value("nodesBlue", 0).toInt());
  palette.setColor(QPalette::Window, QColor(loaded_color));
  ui->color_nodes_widget->setPalette(palette);
  ui->color_nodes_widget->setAutoFillBackground(true);
}

void ModelViewer::on_pushButton_screencast_clicked() {
  ui->pushButton_screencast->setDisabled(true);
  ui->pushButton_screencast->setText("In progress...");

  QString gif_fileName = QFileDialog::getSaveFileName(this, tr("Save gif"), ".",
                                                      tr("GIF files (*.gif)"));

  if (!gif_fileName.isEmpty()) {
    int gif_width = ui->MeshGLWidget->width();
    int gif_height = ui->MeshGLWidget->height();

    // Запуск процесса записи GIF
    startGifRecording(gif_fileName, gif_width, gif_height);
  }
}

void ModelViewer::startGifRecording(const QString &gif_fileName, int gif_width,
                                    int gif_height) {
  QVector<QImage> frames;

  int frameCount = 0;
  const int targetFrameCount =
      10 * 5;  // 10 кадров в секунду в течение 5 секунд

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this,
          [this, frames, frameCount, gif_fileName, gif_width, gif_height,
           timer]() mutable {
            // Снимок рабочей области и добавление его в вектор
            frames.push_back(ui->MeshGLWidget->grabFramebuffer());
            if (!frames.isEmpty()) {
              const QImage &firstFrame = frames.first();
              QString fileName =
                  QDir::homePath() +
                  "/Desktop/first_frame.png";  // Полный путь к файлу на рабочем
                                               // столе
              firstFrame.save(fileName);
            }
            ++frameCount;

            // Проверка условия завершения записи
            if (frameCount >= targetFrameCount) {
              // Остановка таймера
              timer->stop();

              // Включение кнопки
              ui->pushButton_screencast->setDisabled(false);

              // Возвращение текста кнопки в исходное состояние
              ui->pushButton_screencast->setText("Screencast");

              // Сохранение GIF-анимации
              saveGifAnimation(gif_fileName, gif_width, gif_height, frames);
              // Очистка вектора с кадрами
              frames.clear();
            }
          });

  // Запуск таймера
  timer->start(100);
}

void ModelViewer::saveGifAnimation(const QString &gif_fileName, int gif_width,
                                   int gif_height,
                                   const QVector<QImage> &frames) {
  GifWriter gifWriter;
  GifBegin(&gifWriter, gif_fileName.toStdString().c_str(), gif_width,
           gif_height, 10);
  gif_width = 640;
  gif_height = 480;
  for (const QImage &frame : frames) {
    // Изменение размера фрейма до 640x480
    QImage resizedFrame =
        frame.scaled(gif_width, gif_height, Qt::KeepAspectRatio);

    std::vector<uint8_t> rgbaPixels(gif_width * gif_height * 4, 0);

    // Конвертация RGBA пикселей в вектор uint8_t
    for (int y = 0; y < gif_height; ++y) {
      for (int x = 0; x < gif_width; ++x) {
        QRgb pixel = resizedFrame.pixel(x, y);
        rgbaPixels[(y * gif_width + x) * 4 + 0] = qRed(pixel);
        rgbaPixels[(y * gif_width + x) * 4 + 1] = qGreen(pixel);
        rgbaPixels[(y * gif_width + x) * 4 + 2] = qBlue(pixel);
        rgbaPixels[(y * gif_width + x) * 4 + 3] = 0;  // ignoring by gif.h :D
      }
    }

    GifWriteFrame(&gifWriter, rgbaPixels.data(), gif_width, gif_height, 10);
  }

  GifEnd(&gifWriter);
}

void ModelViewer::onTimeout() {
  ui->pushButton_screencast->setText("Screencast");
  ui->pushButton_screencast->setDisabled(false);
}

void ModelViewer::on_radioButton_central_projection_clicked() {
  ui->MeshGLWidget->setCentralPerspective(true);
}

void ModelViewer::on_radioButton_parallel_projection_clicked() {
  ui->MeshGLWidget->setCentralPerspective(false);
}

}  // namespace ViewerFrontend
