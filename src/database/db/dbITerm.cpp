#include "dbITerm.h"

#include <iostream>

namespace db
{

dbITerm::dbITerm()
	: net_   (nullptr),
		inst_  (nullptr),
    mterm_ (nullptr)
{
}

void
dbITerm::print() const
{
  std::cout << "Instance Pin" << std::endl;
	std::cout << "INST    : " << inst_->name() << std::endl;
	std::cout << "NET     : " << net_->name() << std::endl;
	std::cout << "LEF PIN : " << mterm_->name() << std::endl;
	std::cout << std::endl;
}

}
