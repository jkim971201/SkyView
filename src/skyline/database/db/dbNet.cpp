#include "dbNet.h"

#include <iostream>

namespace db
{

dbNet::dbNet()
  : name_ ("")
{
  iterms_.clear();
  bterms_.clear();
}

void
dbNet::print() const
{
	std::cout << std::endl;
  std::cout << "Net name : " << name_ << std::endl;

	for(auto iterm : iterms_)
		std::cout << "ITerm : " << iterm->name() << std::endl;

	for(auto bterm : bterms_)
		std::cout << "BTerm : " << bterm->name() << std::endl;
}

}
