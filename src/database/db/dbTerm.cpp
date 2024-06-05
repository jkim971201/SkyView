#include "dbTerm.h"

#include <iostream>

namespace db
{

dbTerm::dbTerm()
	: pin_  (nullptr),
	  net_  (nullptr),
		inst_ (nullptr)
{
}

void
dbTerm::print() const
{
  std::cout << "Instance Pin" << std::endl;
	std::cout << "INST    : " << inst_->name() << std::endl;
	std::cout << "NET     : " << net_->name() << std::endl;
	std::cout << "LEF PIN : " << pin_->name() << std::endl;
	std::cout << std::endl;
}

}
