#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>

#include "MDI_Interface.h"
#include "db/dbDatabase.h"

using namespace db;

namespace gui
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  
  void display();
  void linkDatabase(std::shared_ptr<dbDatabase> db);

private:

  QApplication* app_;

  std::shared_ptr<dbDatabase> db_;

private slots:
  void newFile();
  void open();
};

}

#endif // MAINWINDOW_H
