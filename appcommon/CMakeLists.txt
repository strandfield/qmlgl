
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(qmlgl-appcommon STATIC ${HDR_FILES} ${SRC_FILES} "qmlglappcommon.qrc")
target_include_directories(qmlgl-appcommon PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(qmlgl-appcommon qmlgl)
