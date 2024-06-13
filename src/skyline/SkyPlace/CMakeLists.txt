cmake_minimum_required(VERSION 3.1)

project(SkyPlace LANGUAGES CXX CUDA)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)

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
add_library(SkyPlace
	src/SkyPlace.cpp
	src/SkyPlaceDB.cpp
  src/InitialPlacer.cpp
  src/FMPartitioner.cpp
  src/Painter.cpp
)

# Include Directory
target_include_directories(${PROJECT_NAME}
	PRIVATE
	db
	${CMAKE_CURRENT_SOURCE_DIR}/src
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
	PRIVATE
	db
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
                   src/CUDA_WL_GRAD.cu
                   src/CUDA_NESTEROV.cu
                   src/CUDA_ADAM.cu
                   src/CUDA_POISSON_SOLVER.cu
                   src/CUDA_DCT.cu
                   src/CUDA_DENSITY_GRAD.cu
									 src/CUDA_INITIAL_PLACE.cu
                   src/CUDA_TARGET_FUNCTION.cu)

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
