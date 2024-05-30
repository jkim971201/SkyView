#include "gui/SkyView.h"
#include "MainWindow.h"

#include <QApplication>

extern int    cmd_argc;
extern char** cmd_argv; 

namespace gui
{

SkyView::SkyView(std::shared_ptr<dbDatabase> db)
  : db_ (db)
{
}

SkyView::~SkyView()
{
}

void
SkyView::display()
{
  QApplication app(cmd_argc, cmd_argv);
  MainWindow window;
  window.setDatabase(db_);
  window.init();
  window.getView()->zoomFit();
  window.show();
  int exit_code = app.exec();
  exit(exit_code);
}

}
