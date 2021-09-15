# KMS TEAM
cmake_minimum_required(VERSION 3.8)

include(CheckLanguage)
check_language(CUDA)
if (CMAKE_CUDA_COMPILER)
    project(kms_eye LANGUAGES CXX CUDA)
else()
    project(kms_eye LANGUAGES CXX)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(JSON_BuildTests OFF CACHE INTERNAL "")
list(APPEND QML_IMPORT_PATH "/home/lacie/Qt/5.15.2/gcc_64/qml")
SET(CMAKE_PREFIX_PATH "/home/lacie/Qt/5.15.2/gcc_64")

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    CACHE STRING "Qt Creator 4.1 extra qml import paths"
    FORCE
)

#set(Qt5_DIR "/opt/Qt/5.15.0")

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)

find_package(Qt5Qml REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

find_package(Qt53DCore REQUIRED)
find_package(Qt53DQuick REQUIRED)
find_package(Qt53DRender REQUIRED)
find_package(Qt53DQuickRender REQUIRED)
find_package(Qt53DInput REQUIRED)
find_package(Qt53DLogic REQUIRED)
find_package(Qt53DQuickInput REQUIRED)


#find_package(CUDA REQUIRED)
#set(CUDA_LINK_LIBRARIES_KEYWORD PUBLIC)
## cuda_select_nvcc_arch_flags(ARCH_FLAGS)
## COMPILER FLAGS AND DEFINITIONS
#set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode=arch=compute_50,code=sm_50)
#set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-std=c++11 -O3)
#message(STATUS "Found CUDA ${CUDA_VERSION_STRING} at ${CUDA_TOOLKIT_ROOT_DIR}")
#include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ${CUDA_INCLUDE_DIRS})

# OpenCV
find_package(OpenCV 4.2 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

## Ceres Solver
#find_package(Ceres REQUIRED)
#include_directories(${CERES_INCLUDE_DIRS})

# PCL
set(PCL_INCLUDE_DIRS /usr/local/include/pcl-1.8 /usr/include/eigen3/)
find_package(VTK REQUIRED)
find_package(PCL 1.8 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

# g2o
find_package(G2O REQUIRED)
find_package(CSparse REQUIRED)
include_directories(${G2O_INCLUDE_DIR} ${CSPARSE_INCLUDE_DIR})

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Model/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Plugin/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/View/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Handler/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Plugin/Q3DPointCloudRender/*.h
                      #${CMAKE_CURRENT_SOURCE_DIR}/hdr/Plugin/QPixmapContainer/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Plugin/QImageItem/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Common/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/*.h)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/Handler/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/Plugin/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/Plugin/Q3DPointCloudRender/*.cpp
                      #${CMAKE_CURRENT_SOURCE_DIR}/src/Plugin/QPixmapContainer/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/Plugin/QImageItem/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES}) 
                      
qt5_add_resources(QT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/share/qml.qrc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hdr/Model
                    ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Plugin
                    ${CMAKE_CURRENT_SOURCE_DIR}/hdr/View
                    ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Handler
                    ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Common
                    ${CMAKE_CURRENT_SOURCE_DIR}/hdr
                    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/visual_odom/include)

add_executable(kms_eye
                ${HEADERS}
                ${SOURCES}
                ${QT_RESOURCES})

target_compile_definitions(kms_eye
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG> )
target_compile_definitions(kms_eye PUBLIC WITH_PCL=1)
target_link_libraries(kms_eye
  PRIVATE Qt5::Core
          Qt5::Quick
          Qt5::Qml
          Qt5::Widgets
          Qt5::3DCore
          Qt5::3DQuick
          Qt5::3DRender
          Qt5::3DInput
          Qt5::3DQuickRender
          Qt5::3DLogic
          Qt5::3DQuickInput
          ${OpenCV_LIBS}
#          ${CUDA_LIBRARIES}
#          ${CERES_LIBRARIES}
          ${PCL_LIBRARIES}
          ${G2O_LIBS}
          ${G2O_CORE_LIBRARY}
          ${G2O_STUFF_LIBRARY}
          g2o_types_slam3d
          cholmod
          ${CSPARSE_LIBRARY}
          stdc++
          -lpcl_filters
          ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/visual_odom/lib/libvisual_odom.so)

#qt5_use_modules(kms_eye Core Qml Widgets Quick 3DCore 3DQuick 3DRender 3DQuickRender 3DInput 3DLogic 3DQuickInput)

## Set different compiler options for cxx and nvcc

#set(cxx_options -Wall -Wextra -Wfatal-errors -ffast-math -fsee -fno-signed-zeros -fno-math-errno -funroll-loops -fno-finite-math-only -march=native -O3 -DNDEBUG)
#set(nvcc_options --device-w --ftz=true --prec-div=false --prec-sqrt=false --fmad=true --default-stream per-thread -O3 -DNDEBUG)

## the generator is only expanded if <COMPILE_LANGUAGE:FOO> evaluates to true

#if (CMAKE_CUDA_COMPILER)
#   target_compile_options(kms_eye PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${cxx_options}> $<$<COMPILE_LANGUAGE:CUDA>:${nvcc_options}>)
#else()
#   target_compile_options(kms_eye PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${cxx_options}> $<$<NOT:$<COMPILE_LANGUAGE:CXX>>:${nvcc_options}>)
#endif()

