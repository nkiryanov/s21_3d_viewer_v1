#include <QApplication>

#include "frontend/model_viewer.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  ModelViewer w;
  w.show();
  return a.exec();
}
