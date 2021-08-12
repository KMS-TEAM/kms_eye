cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LIBSGM_SHARED true)

set(CUDA_ARCH "-arch=sm_50" CACHE STRING "Value of the NVCC -arch option.")
if(${CMAKE_VERSION} VERSION_EQUAL 3.7 OR ${CMAKE_VERSION} VERSION_GREATER 3.7)
  option(AUTO_DETECT_ARCH "Detect local GPU compute arch automatically" ON)
endif()

project(libSGM VERSION 2.7.0)

if(BUILD_OPENCV_WRAPPER)
	find_package(OpenCV REQUIRED core)
	include_directories(${OpenCV_INCLUDE_DIRS})
endif()

configure_file(${PROJECT_SOURCE_DIR}/include/libsgm_config.h.in
               ${PROJECT_SOURCE_DIR}/include/libsgm_config.h
)

add_subdirectory(src)


