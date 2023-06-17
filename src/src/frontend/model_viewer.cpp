#include "frontend/model_viewer.hpp"

#include "frontend/object_gl_widget.hpp"
#include "ui/ui_model_viewer.h"

namespace ViewerFrontend {

ModelViewer::ModelViewer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ModelViewer) {
  ui->setupUi(this);
}

ModelViewer::~ModelViewer() { delete ui; }

}  // namespace ViewerFrontend
