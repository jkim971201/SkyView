%{

#include "skyline/SkyLine.h"


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
read_lef_cmd(const char* filename)
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->readLef(filename);
}

void
read_def_cmd(const char* filename)
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->readDef(filename);
}

void
read_verilog_cmd(const char* filename)
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->readVerilog(filename);
}

void
read_bookshelf_cmd(const char* filename)
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->readBookShelf(filename);
}

void
set_top_module_name_cmd(const char* topname)
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->setTopModuleName(topname);
}

void
global_place_cmd()
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->runGlobalPlace();
}

void
display_cmd()
{
  skyline::SkyLine* sky = skyline::getSkyLine();
  sky->display();
}

%}
