#include <QApplication>
#include <QSurfaceFormat>

#include "frontend/model_viewer.hpp"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  QSurfaceFormat format;
  format.setVersion(2, 1);
  // format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  ViewerFrontend::ModelViewer window;
  window.show();

  return application.exec();
}
