# KMS TEAM
cmake_minimum_required(VERSION 3.14)

project(kms_eye LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

# OpenCV
find_package(OpenCV 4.2 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${QtQml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${QtQml_DEFINITIONS} ${${Qt5Quick_DEFINITIONS}})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Model/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/Plugin/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/View/*.h
                      ${CMAKE_CURRENT_SOURCE_DIR}/hdr/*.h)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/Plugin/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

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
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick ${OpenCV_LIBS})

