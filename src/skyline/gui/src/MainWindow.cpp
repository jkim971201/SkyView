#include "MainWindow.h"

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QListWidget>
#include <QDockWidget>
#include <QStatusBar>
#include <QDebug>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPainter>
#include <QColor>

#include <cassert>
#include <iostream>

namespace gui
{

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow (parent)
{
}

MainWindow::~MainWindow()
{
}

void
MainWindow::init()
{
  assert(db_ != nullptr);

  setWindowTitle("SkyLine");

  // Menu Bar
  createMenu();

  // Dock
  createDock();

  // Status Bar
  statusBar()->showMessage(tr("Ready"));

  // Window Size

  // this line is only for Qt5
  // I don't know why, but this returns merged size when using multiple monitors.
  QSize screenSize = QGuiApplication::primaryScreen()->size();

  // if-else to handle the problem above.
  QSize size = (screenSize.width() > 5000) ? screenSize * 0.4 
                                           : screenSize * 0.8;
  resize(size);

  // Scene
  layout_scene_ = new LayoutScene;
  layout_scene_->setBackgroundBrush( Qt::black );
  layout_scene_->setDatabase(db_);

	// View
  layout_view_ = new LayoutView;
  layout_view_->setScene(layout_scene_);
  setCentralWidget(layout_view_);

  // Draw Objects
  createItem();
}

void
MainWindow::createMenu()
{
  QMenu* fileMenu;
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu = menuBar()->addMenu(tr("&View"));
  fileMenu = menuBar()->addMenu(tr("&Help"));
}

void
MainWindow::createDock()
{
  QDockWidget* dock = new QDockWidget(tr("Object"), this);
  dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  QListWidget* objectList = new QListWidget(dock);

  QStringList layerList;
  for(const auto layer : db_->getTech()->getLayers())
    layerList.append( QString(layer->name().c_str()) );
  objectList->addItems(layerList);

  dock->setWidget(objectList);
  addDockWidget(Qt::RightDockWidgetArea, dock);
}

void
MainWindow::createItem()
{
  layout_scene_->createGuiDie();
  layout_scene_->createGuiRow();
  layout_scene_->createGuiInst();
}

// [SLOTS]
void 
MainWindow::newFile()
{
  qDebug() << Q_FUNC_INFO;
}

void 
MainWindow::open()
{
  qDebug() << Q_FUNC_INFO;
}

}
