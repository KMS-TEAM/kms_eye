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
set(JSON_BuildTests OFF CACHE INTERNAL "")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

find_package(CUDA REQUIRED)
set(CUDA_LINK_LIBRARIES_KEYWORD PUBLIC)
cuda_select_nvcc_arch_flags(ARCH_FLAGS)
message(STATUS "Found CUDA ${CUDA_VERSION_STRING} at ${CUDA_TOOLKIT_ROOT_DIR}")
include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ${CUDA_INCLUDE_DIRS})

# OpenCV
find_package(OpenCV 4.2 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Model/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Model/sgm/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Plugin/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/View/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/*.h)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/sgm/*.cu
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/sgm/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/Plugin/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES}) 
                      
qt5_add_resources(QT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/share/qml.qrc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hdr/Model
                    ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Plugin
                    ${CMAKE_CURRENT_SOURCE_DIR}/hdr/View
                    ${CMAKE_CURRENT_SOURCE_DIR}/hdr)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(kms_eye
            ${HEADERS}
            ${SOURCES}
            ${QT_RESOURCES}
    )
else()
    if(ANDROID)
        add_library(kms_eye SHARED
                ${HEADERS}
                ${SOURCES}
                ${QT_RESOURCES}
        )
    else()
        add_executable(kms_eye
                ${HEADERS}
                ${SOURCES}
                ${QT_RESOURCES}
        )
    endif()
endif()

target_compile_definitions(kms_eye
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(kms_eye
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick ${OpenCV_LIBS} ${CUDA_LIBRARIES})

# Set different compiler options for cxx and nvcc

set(cxx_options -Wall -Wextra -Wfatal-errors -ffast-math -fsee -fno-signed-zeros -fno-math-errno -funroll-loops -fno-finite-math-only -march=native -O3 -DNDEBUG)
set(nvcc_options --device-w --ftz=true --prec-div=false --prec-sqrt=false --fmad=true --default-stream per-thread -O3 -DNDEBUG)

# the generator is only expanded if <COMPILE_LANGUAGE:FOO> evaluates to true

if (CMAKE_CUDA_COMPILER)
   target_compile_options(kms_eye PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${cxx_options}> $<$<COMPILE_LANGUAGE:CUDA>:${nvcc_options}>)
else()
   target_compile_options(kms_eye PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${cxx_options}> $<$<NOT:$<COMPILE_LANGUAGE:CXX>>:${nvcc_options}>)
endif()

