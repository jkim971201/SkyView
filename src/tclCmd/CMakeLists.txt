add_library(tcl_cmd)

find_package(TCL REQUIRED)

target_sources(tcl_cmd
  PRIVATE
	src/CmdCore.cpp
)

target_include_directories(tcl_cmd
  PUBLIC
	include

	PRIVATE
	${TK_INCLUDE_PATH}
)

target_link_libraries(tcl_cmd
	PRIVATE
  skyline
	${TCL_LIBRARY}
)
