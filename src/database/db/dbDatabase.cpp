#include <cassert>
#include <iostream>

#include "dbDatabase.h"
#include "dbLefReader.h"
#include "dbDefReader.h"
#include "dbVerilogReader.h"

namespace db
{

dbDatabase::dbDatabase()
  : defFile_ ("")
{
  lefList_.clear();  

  // Initialization order matters.
  types_     = std::make_shared<dbTypes>();
  tech_      = std::make_shared<dbTech>(types_);
  design_    = std::make_shared<dbDesign>(types_, tech_);

  lefReader_ = std::make_shared<dbLefReader>(types_, tech_);
  defReader_ = std::make_shared<dbDefReader>(types_, tech_, design_);
  verilogReader_ = std::make_shared<dbVerilogReader>(tech_, design_);
}

void
dbDatabase::readLef(const char* fileName)
{
  std::string filenameStr = std::string(fileName);

  if(lefList_.count(filenameStr))
    return;

  lefList_.insert(filenameStr);

  std::cout << "Read   " << filenameStr << std::endl;

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

  std::cout << "Read   " << filenameStr << std::endl;

  defReader_->parseDef(filenameStr);

  std::cout << "Finish " << filenameStr << std::endl;
}

void
dbDatabase::readVerilog(const char* fileName)
{
  std::string filenameStr = std::string(fileName);

  if(vFile_ != "")
  {
    printf("You cannot read multiple .v files!\n");
    exit(1);
  }

  vFile_ = filenameStr;

  std::cout << "Read   " << filenameStr << std::endl;

  verilogReader_->readFile(filenameStr);

  std::cout << "Finish " << filenameStr << std::endl;
}

}
