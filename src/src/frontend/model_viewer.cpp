#include "frontend/model_viewer.h"

#include "./ui_model_viewer.h"

ModelViewer::ModelViewer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ModelViewer) {
  ui->setupUi(this);
}

ModelViewer::~ModelViewer() { delete ui; }
