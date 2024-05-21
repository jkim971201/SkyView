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
	db_ = std::make_shared<dbDatabase>();
}

SkyLine::~SkyLine()
{
}

void
SkyLine::readLef(const char* file_path)
{
  db_->readLef(file_path);
}

}
