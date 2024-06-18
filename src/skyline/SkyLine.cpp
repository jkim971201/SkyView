#include <cstdlib>
#include <tcl.h>

#include "skyline/SkyLine.h"

#include "db/dbDatabase.h"
#include "gui/SkyView.h"
#include "skyplace/SkyPlace.h"

#include "tclCmd/CmdGlobalPlace.hpp"

namespace skyline
{

// This is a callback function when the tcl interpreter meets a command.
int cmdCbk(ClientData clientData, Tcl_Interp* interp, int objc, struct Tcl_Obj* const* objv)
{
  const char* cmd_name = Tcl_GetString(objv[0]);
	skyline::TclCmd* cmd_ptr = TclCmdList::getCmdByName(cmd_name);

  bool isOptArg = false;
  std::string curOpt = "";

  for(int cnt = 1; cnt < objc; ++cnt) 
  {
    struct Tcl_Obj* obj = objv[cnt];
    const char* obj_str = Tcl_GetString(obj);

    if(obj_str[0] == '-')
    {
      if(isOptArg == true)
      {
        printf("Invalid use of command %s...\n", cmd_name);
        return TCL_ERROR;
      }
      else
      {
        isOptArg = true;
        curOpt = std::string(obj_str);
      }
    }
    else
    {
      if(isOptArg)
      {
        cmd_ptr->giveOptArg(curOpt, atof(obj_str));
        isOptArg = false;
      }
      else
      {
				std::string obj_cpp_str = std::string(obj_str);
        cmd_ptr->giveArg(obj_cpp_str);
      }
    }
  }

  cmd_ptr->execute();

  return TCL_OK;
}

// Static Class Implementation
std::unordered_map<std::string, std::unique_ptr<TclCmd>> TclCmdList::name2Cmd_;

void
TclCmdList::addTclCmd(Tcl_Interp* interp, std::unique_ptr<TclCmd> newCmd)
{
  Tcl_CreateObjCommand(interp, newCmd->name().c_str(), cmdCbk, nullptr, nullptr);
	// We don't use Client Data and Delete Callback function
  name2Cmd_[newCmd->name()] = std::move(newCmd);
}

TclCmd*
TclCmdList::getCmdByName(const char* name)
{
	std::string nameStr = std::string(name);
	auto itr = name2Cmd_.find(nameStr);

	if(itr != name2Cmd_.end())
		return itr->second.get(); // return raw pointer
  else
		return nullptr;
}

/////////////////////////////////////////////////////////////////////
// SkyLine

// Singleton Design Pattern
SkyLine* SkyLine::getStaticPtr()
{
  static SkyLine sky;
  return &sky;
}

SkyLine::SkyLine()
{
  db_       = std::make_shared<db::dbDatabase>();
  gui_      = std::make_unique<gui::SkyView>(db_);
  skyplace_ = std::make_unique<skyplace::SkyPlace>(db_);
}

SkyLine::~SkyLine()
{
}

void
SkyLine::initSkyLine(Tcl_Interp* interp)
{
  interp_ = interp;
}

void
SkyLine::initTclCmds()
{
	TclCmdList::addTclCmd(interp_, std::make_unique<CmdGlobalPlace>("global_place_test"));
}

void
SkyLine::readLef(const char* file_path)
{
  db_->readLef(file_path);
}

void
SkyLine::readDef(const char* file_path)
{
  db_->readDef(file_path);
}

void
SkyLine::readVerilog(const char* file_path)
{
  db_->readVerilog(file_path);
}

void
SkyLine::readBookShelf(const char* file_path)
{
  db_->readBookShelf(file_path);
}

void
SkyLine::setTopModuleName(const char* top_name)
{
  db_->setTopModuleName(top_name);
}

void
SkyLine::runGlobalPlace()
{
  skyplace_->run();
}

void
SkyLine::display()
{
  gui_->display();
}



}
