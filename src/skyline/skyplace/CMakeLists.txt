cmake_minimum_required(VERSION 3.1)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON) # For CImg

# Home Directory
set(Placer_HOME ${CMAKE_CURRENT_SOURCE_DIR})
set(CIMG_HOME ${CMAKE_CURRENT_SOURCE_DIR}/extern/CImg)
set(Louvain_HOME ${CMAKE_CURRENT_SOURCE_DIR}/extern/Louvain)
set(EIGEN_HOME ${CMAKE_CURRENT_SOURCE_DIR}/extern/eigen-git-mirror)

add_subdirectory(${Louvain_HOME})

# For CImg
find_package(X11 REQUIRED)
find_package(JPEG REQUIRED)
find_package(Threads REQUIRED)

# Source Code
add_library(skyplace)

target_sources(skyplace
	PRIVATE
  src/SkyPlace.cpp
  src/SkyPlaceDB.cpp
  src/InitialPlacer.cpp
  src/FMPartitioner.cpp
  src/Painter.cpp
)

# Include Directory
target_include_directories(skyplace
  PUBLIC
  include 
  PRIVATE
  ${X11_INCLUDE_DIR}
  ${CIMG_HOME}
  ${EIGEN_HOME}
  ${Louvain_HOME}
  /tool/cplex/install/2211/cplex/include
  /tool/cplex/install/2211/concert/include
  /home/jkim/mosek/10.1/tools/platform/linux64x86/h
)

# Link Library
target_link_libraries(skyplace
  PRIVATE
  db
  ${X11_LIBRARIES}
  Threads::Threads
  Louvain
  /home/jkim/mosek/10.1/tools/platform/linux64x86/bin/libmosek64.so.10.1
  /home/jkim/mosek/10.1/tools/platform/linux64x86/bin/libfusion64.so.10.1
)

# Enable CUDA
find_package(CUDAToolkit)
if (CUDAToolkit_FOUND)
  enable_language(CUDA)

  find_package(Thrust)

  set_target_properties(skyplace PROPERTIES CUDA_ARCHITECTURES "75")
  set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -arch=sm_61")
  set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr")

  target_sources(skyplace
                 PRIVATE
                 src/CUDA_WL_GRAD.cu
                 src/CUDA_NESTEROV.cu
                 src/CUDA_ADAM.cu
                 src/CUDA_POISSON_SOLVER.cu
                 src/CUDA_DCT.cu
                 src/CUDA_DENSITY_GRAD.cu
                 src/CUDA_TARGET_FUNCTION.cu)

  target_link_libraries(skyplace
                        PRIVATE
                        CUDA::cudart
                        CUDA::cublas
                        CUDA::cusparse
                        CUDA::cufft
                        CUDA::cusolver)

else()
  message(STATUS "CUDA Not Found")
endif()
