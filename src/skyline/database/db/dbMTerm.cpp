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
dbMTerm::setBoundary()
{
	if(rects_.empty())
		return;

	lx_ = std::numeric_limits<int>::max();
	ly_ = std::numeric_limits<int>::max();
	ux_ = std::numeric_limits<int>::min();
	uy_ = std::numeric_limits<int>::min();

	for(auto& rect : rects_)
  {
		int newLx = rect.lx;
		int newLy = rect.ly;
		int newUx = rect.ux;
		int newUy = rect.uy;

    if(newLx < lx_) lx_ = newLx;
    if(newLy < ly_) ly_ = newLy;
    if(newUx > ux_) ux_ = newUx;
    if(newUy > uy_) uy_ = newUy;
  }
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
