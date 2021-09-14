cmake_minimum_required(VERSION 2.8)

project(visual_odom)

option(USE_CUDA "Using CUDA to accelerate" OFF)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Eigen3
include_directories("/usr/include/eigen3")
find_package (Eigen3 3.3 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include/visual_odom)

if(USE_CUDA)
    find_package(CUDA)
    include_directories("${CUDA_INCLUDE_DIRS}")
    add_definitions(-DUSE_CUDA)
endif(USE_CUDA)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(visual_odom SHARED ${SOURCES})

target_link_libraries(visual_odom ${OpenCV_LIBS})

add_subdirectory(test)
