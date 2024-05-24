#include "dbIO.h"

#include <cassert>

namespace db
{

dbIO::dbIO()
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
dbIO::setLocation()
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
dbIO::print() const
{
}

}
