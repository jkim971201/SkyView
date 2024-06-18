#include <cstdio>

#include "dbDie.h"

namespace db
{

dbDie::dbDie()
: lx_ (0),
  ly_ (0),
  ux_ (0),
  uy_ (0)
{}

void
dbDie::print() const
{
  printf("DIEAREA\n");
  printf("(%d, %d)\n", lx_, ly_);
  printf("(%d, %d)\n", ux_, uy_);
  printf("\n");
}

}
