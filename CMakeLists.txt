cmake_minimum_required(VERSION 3.1)

project(Viewer VERSION 1 LANGUAGES CXX)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O3 -DIL_STD -ldl")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O3 -DIL_STD -ldl")

# Home Directory
set(PROJECT_HOME ${PROJECT_SOURCE_DIR})

add_subdirectory(${PROJECT_HOME}/src/database)

# Source Code
set(PROJECT_SRC src/main.cpp)

# Executable
add_executable(Viewer ${PROJECT_SRC})

# Include Directory
target_include_directories(${PROJECT_NAME}
  PUBLIC
	src/database
)

# Link Library
target_link_libraries(${PROJECT_NAME} 
  PUBLIC
	db
)
