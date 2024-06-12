#include <iostream>

#include "dbRow.h"

namespace db
{

dbRow::dbRow()
  : name_      (""),
    site_      (nullptr),
    origX_     (0),
    origY_     (0),
    numSiteX_  (0),
    numSiteY_  (0),
    stepX_     (0),
    stepY_     (0)
{
  orient_ = Orient::N;
}

void
dbRow::print() const
{
  std::cout << std::endl;
  std::cout << "ROW " << name_ << std::endl;
  std::cout << "SITE : " << site_->name() << std::endl;
  std::cout << "ORIGIN : " << origX_ << " " << origY_ << std::endl;
  std::cout << "Orient : " << orient_ << std::endl;
  std::cout << "NumSite : " << numSiteX_ << " BY " << numSiteY_ << std::endl;
  std::cout << "STEP : " << stepX_ << " " << stepY_ << std::endl;
  std::cout << std::endl;
}

}
