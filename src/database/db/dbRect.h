#pragma once

#include <cassert>

namespace db
{

class dbLayer;

// This class is when
// #1. for describing the pin shape of LefPin
// #2. for describing the obstruct shape
class dbRect
{
	public:

  dbRect(int lx, int ly, int ux, int uy, dbLayer* _layer)
    : lx(lx), ly(ly), ux(ux), uy(uy), layer (_layer) 
	{
		assert(_layer != nullptr);
	}

  int lx;
  int ly;
  int ux;
  int uy;
	dbLayer* layer;
};

}
