#include <iostream>

#include "dbMTerm.h"

namespace db
{

dbMTerm::dbMTerm()
  : macro_   (nullptr),
    name_    (""),
    lx_      (0),
    ly_      (0),
    ux_      (0),
    uy_      (0)
{
  rects_.clear();
}

void
dbMTerm::print() const
{
  std::cout << std::endl;
  std::cout << "PIN       : " << name_  << std::endl;
  std::cout << "MACRO     : " << macro_->name() << std::endl;
  std::cout << "DIRECTION : " << pinDir_   << std::endl;
  std::cout << "USAGE     : " << pinUsage_ << std::endl;
  std::cout << "SHAPE     : " << pinShape_ << std::endl;
  for(const auto& r : rects_)
  {
    std::cout << "LAYER: " << r.layer->name() << " ";
    std::cout << "RECT " << r.lx << " " << r.ly << " " << r.ux << " " << r.uy << "\n";
  }
  std::cout << std::endl;
}

}
