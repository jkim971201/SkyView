#include <cstdio>
#include <iostream>
#include <string>
#include <tcl.h>

#include "SkyLine.h"

extern "C" 
{ 
	extern int Skyline_Init(Tcl_Interp* interp); 
}

bool exit_mode = true;
char** cmd_argv;

void sourceTclFile(std::string& filename, Tcl_Interp *interp)
{
  std::string cmd = "source " + filename;
  int code = Tcl_Eval(interp, cmd.c_str());

  const char* result = Tcl_GetStringResult(interp);
  if(result[0] != '\0')
    std::cout << result << std::endl;
  if(exit_mode)
    exit(0);
}

int customTclInit(char** argv, Tcl_Interp* interp)
{
  interp = Tcl_CreateInterp();

  if(Tcl_Init(interp) == TCL_ERROR)
  {
    std::cout << "ERROR - Cannot create Tcl interpreter" << std::endl;
    return TCL_ERROR;
  }
  else
  {
    Skyline_Init(interp);

    #ifdef SKYLINE_TCL
		std::string init_cmd = "source " + std::string(SKYLINE_TCL);
    #else
		std::cout << "SkyLine.tcl is not defined" << std::endl;
		exit(1);
    #endif

		Tcl_Eval(interp, init_cmd.c_str());

    std::string filename = std::string(argv[1]);

    sourceTclFile(filename, interp);

    return TCL_OK;
  }
}

int tclInitWrapper(Tcl_Interp* interp)
{
  return customTclInit(cmd_argv, interp);
}

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    std::cout << "Please give input file" << std::endl;
    exit(0);
  }

  cmd_argv = argv;

  Tcl_Main(1, argv, tclInitWrapper);

  return 0;
}
