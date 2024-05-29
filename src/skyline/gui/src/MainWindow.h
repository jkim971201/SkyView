#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "db/dbDatabase.h"
#include "LayoutView.h"

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

    const std::shared_ptr<dbDatabase> db() const { return db_; }

  private:

    std::shared_ptr<dbDatabase> db_;

    QGraphicsScene* base_scene_;
    LayoutView*     layout_view_;

		void createMenu();
		void createDock();

  private slots:

    void newFile();
    void open();
};

}

#endif // MAINWINDOW_H
