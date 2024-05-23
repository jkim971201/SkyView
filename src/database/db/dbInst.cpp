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
      dx_ = macro_->sizeX();
      dy_ = macro_->sizeY();
			lx_ = placementX;
			ly_ = placementY;
			break;
		}
		case Orient::S :
		{
      dx_ = macro_->sizeX();
      dy_ = macro_->sizeY();
			lx_ = placementX - dx_;
			ly_ = placementY - dy_;
			break;
		}
		case Orient::W :
		{
      dx_ = macro_->sizeY();
      dy_ = macro_->sizeX();
			lx_ = placementX - dx_;
			ly_ = placementY;
			break;
		}
		case Orient::E :
		{
      dx_ = macro_->sizeY();
      dy_ = macro_->sizeX();
			lx_ = placementX;
			ly_ = placementY - dy_;
			break;
		}
		case Orient::FN :
		{
      dx_ = macro_->sizeX();
      dy_ = macro_->sizeY();
			lx_ = placementX - dx_;
			ly_ = placementY;
			break;
		}
		case Orient::FS :
		{
      dx_ = macro_->sizeX();
      dy_ = macro_->sizeY();
			lx_ = placementX;
			ly_ = placementY - dy_;
			break;
		}
		case Orient::FW :
		{
      dx_ = macro_->sizeY();
      dy_ = macro_->sizeX();
			lx_ = placementX;
			ly_ = placementY;
			break;
		}
		case Orient::FE :
		{
      dx_ = macro_->sizeY();
      dy_ = macro_->sizeX();
			lx_ = placementX - dx_;
			ly_ = placementY - dy_;
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
	std::cout << "LX LY : " << lx() << " " << ly() << std::endl;
	std::cout << "UX UY : " << ux() << " " << uy() << std::endl;
  std::cout << std::endl;
}

}
