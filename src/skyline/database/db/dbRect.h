#ifndef DB_RECT_H
#define DB_RECT_H

#include <cassert>

namespace db
{

class dbLayer;

// This class is used when
// #1. for describing the pin shape of LefPin
// #2. for describing the obstruct shape
// #3. for describing the boundary of DEF IO
class dbRect
{
  public:

  dbRect(int lx, int ly, int ux, int uy, dbLayer* _layer)
    : lx(lx), ly(ly), ux(ux), uy(uy), layer (_layer) 
  {
  }

  int lx;
  int ly;
  int ux;
  int uy;
  dbLayer* layer;
};

}

#endif
