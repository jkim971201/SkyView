cmake_minimum_required(VERSION 3.1)

project(Viewer VERSION 1 LANGUAGES CXX)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O3 -DIL_STD -ldl")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O3 -DIL_STD -ldl")

# Home Directory
set(Viewer_HOME ${PROJECT_SOURCE_DIR})

MESSAGE(STATUS "PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")

add_subdirectory(${Viewer_HOME}/src/parsers/lef)

# Source Code
set(Viewer_SRC
	src/db/dbDatabase.cpp
	src/db/dbTech.cpp
	src/db/dbTypes.cpp
	src/main.cpp
)

# Executable
add_executable(Viewer ${Viewer_SRC})

# Include Directory
target_include_directories(${PROJECT_NAME}
	PUBLIC
	${PROJECT_SOURCE_DIR}/src/db
	${PROJECT_SOURCE_DIR}/src/parsers/lef
)

# Link Library
target_link_libraries(${PROJECT_NAME} 
	PUBLIC
	lef
)
