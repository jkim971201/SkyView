#include "Viewer.h"
#include "MainWindow.h"

namespace gui
{

Viewer::Viewer(std::shared_ptr<dbDatabase> db)
	: db_ (db)
{
  window_ = std::make_unique<MainWindow>(nullptr);
}

void
Viewer::display()
{
  window_->display();
}

}
