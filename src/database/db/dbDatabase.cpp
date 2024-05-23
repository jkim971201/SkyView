#include <cassert>
#include <iostream>

#include "dbDatabase.h"

namespace db
{

dbDatabase::dbDatabase()
  : defFile_ ("")
{
  lefList_.clear();  

  // Initialization order matters.
  types_     = std::make_shared<dbTypes>();
  tech_      = std::make_shared<dbTech>(types_);
  lefReader_ = std::make_shared<dbLefReader>(types_, tech_);
  defReader_ = std::make_shared<dbDefReader>(types_, tech_);
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

void
dbDatabase::readDef(const char* fileName)
{
  std::string filenameStr = std::string(fileName);

  if(defFile_ != "")
  {
    printf("You cannot read multiple .def files!\n");
    exit(1);
  }

  defFile_ = filenameStr;

  std::cout << "Read " << filenameStr << std::endl;

  defReader_->parseDef(filenameStr);

  std::cout << "Finish " << filenameStr << std::endl;
}


}
