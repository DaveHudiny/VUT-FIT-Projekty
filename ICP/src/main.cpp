/**
  @file main.cpp
  @author David Hudak
  @brief File contains implementation for class LineConnection, mostly generated
  */

#include "mainwindow.h"
#include "classes.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
