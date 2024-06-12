cmake_minimum_required(VERSION 3.1)

project(Placer VERSION 1
LANGUAGES CXX
)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O3 -DIL_STD -ldl")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O3 -DIL_STD -ldl")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr")

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON) # For CImg

# Home Directory
set(Placer_HOME ${PROJECT_SOURCE_DIR})
set(CIMG_HOME ${PROJECT_SOURCE_DIR}/extern/CImg)
set(Louvain_HOME ${PROJECT_SOURCE_DIR}/extern/Louvain)
set(EIGEN_HOME ${PROJECT_SOURCE_DIR}/extern/eigen-git-mirror)

add_subdirectory(${Louvain_HOME})

# 1. For CImg
find_package(X11 REQUIRED)
find_package(JPEG REQUIRED)
find_package(Threads REQUIRED)

# 2. For Eigen 
#set(EIGEN3_INCLUDE_DIR "/tool/eigen3/install/3.4.0/include/eigen3")
#find_package(Eigen3 REQUIRED)
#MESSAGE( STATUS "Eigen3 Include: ${EIGEN3_INCLUDE_DIR}")

MESSAGE( STATUS "PROJECT HOME : ${PROJECT_SOURCE_DIR}")
MESSAGE( STATUS "Eigen3 HOME   : ${EIGEN_HOME}")
MESSAGE( STATUS "Eigen3 Include: ${EIGEN_INCLUDE_DIR}")

# Source Code
set(Placer_SRC
  src/main.cpp
  src/CmdInterpreter.cpp
  src/db/BookShelfParser.cpp
  src/db/LefDefParser.cpp
  src/placer/Placer.cpp
  src/placer/PlacerDB.cpp
  src/placer/InitialPlacer.cpp
  src/placer/FMPartitioner.cpp
  src/placer/Legalizer.cpp
  src/gui/Painter.cpp
)

# Executable
add_executable(Placer ${Placer_SRC})

# Include Directory
target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  ${X11_INCLUDE_DIR}
  ${CIMG_HOME}
  ${EIGEN_HOME}
  ${Louvain_HOME}
  /tool/cplex/install/2211/cplex/include
  /tool/cplex/install/2211/concert/include
  /home/jkim/mosek/10.1/tools/platform/linux64x86/h
)

# Link Library
target_link_libraries(${PROJECT_NAME} 
  PUBLIC
   ${X11_LIBRARIES}
  Threads::Threads
  Louvain
  /home/jkim/mosek/10.1/tools/platform/linux64x86/bin/libmosek64.so.10.1
  /home/jkim/mosek/10.1/tools/platform/linux64x86/bin/libfusion64.so.10.1
)

# Enable CUDA
option(GPU "Enable GPU" ON)
if (GPU)
  message(STATUS "GPU Mode ON")
  find_package(CUDAToolkit)
  if (CUDAToolkit_FOUND)
    message(STATUS "CUDA Found")
    enable_language(CUDA)

    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --use_fast_math")

    find_package(Thrust)

    set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_ARCHITECTURES "75")
    string(APPEND CMAKE_CUDA_FLAGS " -arch=sm_61")

    target_sources(${PROJECT_NAME} 
                   PRIVATE
                   src/placer/CUDA_WL_GRAD.cu
                   src/placer/CUDA_NESTEROV.cu
                   src/placer/CUDA_ADAM.cu
                   src/placer/CUDA_POISSON_SOLVER.cu
                   src/placer/CUDA_DCT.cu
                   src/placer/CUDA_DENSITY_GRAD.cu
                   src/placer/CUDA_CLUSTERING.cu
                   src/placer/CUDA_TARGET_FUNCTION.cu)

    target_link_libraries(${PROJECT_NAME} 
                          PRIVATE
                          CUDA::cudart
                          CUDA::cublas
                          CUDA::cusparse
                          CUDA::cufft
                          CUDA::cusolver)

    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_GPU)

  else()
    message(STATUS "CUDA Not Found")
  endif()
else()
  message(STATUS "GPU Mode Off")
endif()
