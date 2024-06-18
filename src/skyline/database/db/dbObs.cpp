#include <iostream>

#include "dbObs.h"

namespace db
{

dbObs::dbObs()
{
  rects_.clear();
}

void
dbObs::print() const
{
  std::cout << std::endl;
	std::cout << "OBS" << std::endl;
  for(const auto& r : rects_)
  {
    std::cout << "LAYER: " << r.layer->name() << " ";
    std::cout << "RECT " << r.lx << " " << r.ly << " " << r.ux << " " << r.uy << "\n";
  }
  std::cout << std::endl;
}

}
