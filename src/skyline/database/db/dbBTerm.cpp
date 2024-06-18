#include "dbBTerm.h"

#include <iostream>
#include <cassert>

namespace db
{

dbBTerm::dbBTerm()
 : name_     (""),
   lx_       (0),
   ly_       (0),
   ux_       (0),
   uy_       (0),
   origX_    (0),
   origY_    (0),
   net_      (nullptr)
{
  orient_    = Orient::N;
  direction_ = PinDirection::INOUT;
  status_    = Status::PLACED;
  rects_.clear();
}

void
dbBTerm::setLocation()
{
  assert( rects_.size() > 0 );

  const auto& rect = rects_[0];
  switch(orient_)
  {
    case Orient::N :
    {
      lx_ = origX_ + rect.lx;
      ly_ = origY_ + rect.ly;
      ux_ = origX_ + rect.ux;
      uy_ = origY_ + rect.uy;
      break;
    }
    case Orient::S :
    {
      assert(0);
      break;
    }
    case Orient::W :
    {
      assert(0);
      break;
    }
    case Orient::E :
    {
      assert(0);
      break;
    }
    case Orient::FN :
    {
      assert(0);
      break;
    }
    case Orient::FS :
    {
      assert(0);
      break;
    }
    case Orient::FW :
    {
      assert(0);
      break;
    }
    case Orient::FE :
    {
      assert(0);
      break;
    }
  }
}

void
dbBTerm::print() const
{
  std::cout << std::endl;
  std::cout << "PIN NAME  : " << name_ << std::endl;
  std::cout << "NET NAME  : " << net_->name() << std::endl;
  std::cout << "LAYER : " << rects_[0].layer->name() << std::endl;
  std::cout << "LX LY : " << lx() << " " << ly() << std::endl;
  std::cout << "UX UY : " << ux() << " " << uy() << std::endl;
  std::cout << std::endl;
}

}
