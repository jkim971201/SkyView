#include <iostream>

#include "dbInst.h"

namespace db
{

dbInst::dbInst()
  : macro_ (nullptr),
    name_  (""),
    lx_    (0),
    ly_    (0),
    dx_    (0),
    dy_    (0),
    haloT_ (0),
    haloB_ (0),
    haloL_ (0),
    haloR_ (0)
{
  orient_ = Orient::N;
  source_ = Source::NETLIST;
  status_ = Status::UNPLACED;
}

void 
dbInst::setLocation(int placementX, int placementY)
{
  switch(orient_)
	{
		case Orient::N :
		{
			break;
		}
		case Orient::S :
		{
			break;
		}
		case Orient::W :
		{
			break;
		}
		case Orient::E :
		{
			break;
		}
		case Orient::FN :
		{
			break;
		}
		case Orient::FS :
		{
			break;
		}
		case Orient::FW :
		{
			break;
		}
		case Orient::FE :
		{
			break;
		}
	}
}

void
dbInst::print() const
{
  std::cout << std::endl;
  std::cout << "MACRO : " << macro_->name() << std::endl;
  std::cout << "NAME  : " << name_ << std::endl;
  std::cout << std::endl;
}

}
