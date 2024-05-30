#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "db/dbDatabase.h"
#include "LayoutView.h"
#include "LayoutScene.h"

using namespace db;

namespace gui
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
  
    void init();
    void setDatabase(std::shared_ptr<dbDatabase> db) { db_ = db; }

    LayoutScene* getScene() { return layout_scene_; }
    LayoutView*  getView()  { return layout_view_;  }

  private:

    std::shared_ptr<dbDatabase> db_;

    LayoutScene* layout_scene_;
    LayoutView*  layout_view_;

    void createMenu();
    void createDock();
		void createItem();

  private slots:

    void newFile();
    void open();
};

}

#endif // MAINWINDOW_H
