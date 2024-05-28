%{

#include "SkyLine.h"


namespace skyline
{

SkyLine*
getSkyLine()
{
  return SkyLine::getStaticPtr();
}

}

%}

%inline %{

void
read_lef_cmd(const char *filename)
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->readLef(filename);
}

void
read_def_cmd(const char *filename)
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->readDef(filename);
}

void
display_cmd()
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->display();
}

%}
