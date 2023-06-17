#include <QApplication>

#include "frontend/model_viewer.hpp"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  ViewerFrontend::ModelViewer window;
  window.show();

  return application.exec();
}
