#include "gui/SkyView.h"
#include "MainWindow.h"

extern int    cmd_argc;
extern char** cmd_argv; 

namespace gui
{

SkyView::SkyView(const std::shared_ptr<dbDatabase> db)
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
  window.show();
  int exit_code = app.exec();
  exit(exit_code);
}

}
