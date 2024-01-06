cmake_minimum_required(VERSION 3.9)
project(qmlgl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##################################################################
####### Qt
##################################################################

find_package(Qt5 COMPONENTS Core Gui Widgets Quick Qml REQUIRED)

##################################################################
####### qmlgl library
##################################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC TRUE)

file(GLOB_RECURSE LIBQMLGL_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/*.cpp)
file(GLOB_RECURSE LIBQMLGL_HDR_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/*.h
)
add_library(qmlgl STATIC ${LIBQMLGL_HDR_FILES} ${LIBQMLGL_SRC_FILES})
target_include_directories(qmlgl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/include")
target_include_directories(qmlgl PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/include/qmlgl")
target_link_libraries(qmlgl Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml)

target_compile_definitions(qmlgl PRIVATE -DLIBQMLGL_BUILD_LIBRARY)
target_compile_definitions(qmlgl PUBLIC -DLIBQMLGL_STATIC_LINKING)

##################################################################
####### apps
##################################################################

set(QMLGL_BUILD_APPS FALSE CACHE BOOL "Build the example applications")

if(QMLGL_BUILD_APPS)
  add_subdirectory(appcommon)
  add_subdirectory(apps)
endif()
