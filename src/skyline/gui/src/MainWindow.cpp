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
  setWindowTitle("SkyLine");

  // Menu Bar
  createMenu();

  // Dock
	createDock();

  // Status Bar
  statusBar()->showMessage(tr("Ready"));

  // Window Size
  // For Qt5
  // I don't know why, but this returns merged size when using multiple monitors.
  QSize screenSize = QGuiApplication::primaryScreen()->size();

  // if-else to handle the problem above.
  QSize size = (screenSize.width() > 5000) ? screenSize * 0.4 
                                           : screenSize * 0.8;
  resize(size);

  // LayoutView
  base_scene_ = new QGraphicsScene();
  base_scene_->setBackgroundBrush( Qt::black );

  layout_view_ = new LayoutView;
  layout_view_->setScene(base_scene_);
  setCentralWidget(layout_view_);
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
