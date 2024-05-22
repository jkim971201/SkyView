#include <iostream>

#include "dbPin.h"

namespace db
{

dbPin::dbPin()
  : dbMacro_ (nullptr),
    dbLayer_ (nullptr),
    pinName_ (""),
    lx_      (0),
    ly_      (0),
    ux_      (0),
    uy_      (0)
{
  dbRect_.clear();
}

void
dbPin::print() const
{
  std::cout << std::endl;
  std::cout << "NAME      : " << pinName_  << std::endl;
  std::cout << "DIRECTION : " << pinDir_   << std::endl;
  std::cout << "USAGE     : " << pinUsage_ << std::endl;
  std::cout << "SHAPE     : " << pinShape_ << std::endl;
  for(const auto& r : dbRect_)
    std:: cout << "RECT " << r.lx << " " << r.ly << " " << r.ux << " " << r.uy << "\n";
  std::cout << std::endl;
}

}
