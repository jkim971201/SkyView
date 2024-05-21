#include <cassert>
#include <iostream>

#include "dbDatabase.h"

namespace db
{

dbDatabase::dbDatabase()
{
  // Initialization order matters.
  types_     = std::make_shared<dbTypes>();
  tech_      = std::make_shared<dbTech>(types_);
  lefReader_ = std::make_shared<dbLefReader>(types_, tech_);
}

void
dbDatabase::readLef(const char* fileName)
{
  std::string filenameStr = std::string(fileName);

  if(lefList_.count(filenameStr))
    return;

  lefList_.insert(filenameStr);

  std::cout << "Read " << filenameStr << std::endl;

  lefReader_->parseLef(filenameStr);

  std::cout << "Finish " << filenameStr << std::endl;
}

}
