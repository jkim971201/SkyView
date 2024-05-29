#include "gui/SkyView.h"
#include "MainWindow.h"

namespace gui
{

SkyView::SkyView()
  : db_     (nullptr),
    app_    (nullptr),
    window_ (nullptr)
{
  int    argc = 1;
  char** argv = nullptr;
  app_    = std::make_unique<QApplication>(argc, argv);
  window_ = std::make_unique<MainWindow>();
}

SkyView::~SkyView()
{
}

void
SkyView::linkDatabase(std::shared_ptr<dbDatabase> db)
{
  db_ = db;
}

void
SkyView::display()
{
  window_->display();
}

}
