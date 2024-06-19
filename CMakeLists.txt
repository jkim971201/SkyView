cmake_minimum_required(VERSION 3.1)

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

project(Viewer LANGUAGES CXX)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Home Directory
add_subdirectory(${PROJECT_SOURCE_DIR}/src/skyline)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/tclCmd)

# Source Code
set(PROJECT_SRC 
	src/main.cpp
)

# Executable
add_executable(Viewer ${PROJECT_SRC})

# Compile Definition
target_compile_definitions(Viewer 
  PUBLIC 
)

# Include Directory
target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${TK_INCLUDE_PATH} 
)

# Link Library
target_link_libraries(${PROJECT_NAME} 
  PUBLIC
  ${TCL_LIBRARY}
	skyline
  tcl_cmd	
)
