#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>

#include "MDI_Interface.h"

namespace gui
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  
  void display();

private:

  QApplication* app_;

  int    argc_;
  char** argv_;

private slots:
  void newFile();
  void open();
};

}

#endif // MAINWINDOW_H
