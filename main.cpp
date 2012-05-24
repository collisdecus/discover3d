#include "mainwindow.h"
#include <QtGui/QApplication>

int main(int argc, char* argv[])
{
  QApplication application(argc, argv);
  MainWindow window;
  window.show();
    
  return application.exec();
}
