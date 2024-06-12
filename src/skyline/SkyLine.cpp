#include "SkyLine.h"

namespace skyline
{

// This techniques is called "Singleton Design Pattern" in OOP
SkyLine* SkyLine::getStaticPtr()
{
  static SkyLine sky;
  return &sky;
}

SkyLine::SkyLine()
{
  db_  = std::make_shared<dbDatabase>();
  gui_ = std::make_shared<SkyView>(db_);
}

SkyLine::~SkyLine()
{
}

void
SkyLine::readLef(const char* file_path)
{
  db_->readLef(file_path);
}

void
SkyLine::readDef(const char* file_path)
{
  db_->readDef(file_path);
}

void
SkyLine::readVerilog(const char* file_path)
{
  db_->readVerilog(file_path);
}

void
SkyLine::readBookShelf(const char* file_path)
{
  db_->readBookShelf(file_path);
}

void
SkyLine::setTopModuleName(const char* top_name)
{
  db_->setTopModuleName(top_name);
}

void
SkyLine::display()
{
  gui_->display();
}

}
