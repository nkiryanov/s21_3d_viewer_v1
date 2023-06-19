#include "frontend/model_viewer.hpp"

#include "frontend/object_gl_widget.hpp"
#include "ui/ui_model_viewer.h"

inline void iniModelViewerResources() { Q_INIT_RESOURCE(resources); }

namespace ViewerFrontend {

ModelViewer::ModelViewer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ModelViewer) {
  iniModelViewerResources();

  ui->setupUi(this);
}

ModelViewer::~ModelViewer() { delete ui; }

}  // namespace ViewerFrontend
