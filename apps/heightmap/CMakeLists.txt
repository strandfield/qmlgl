
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(qmlgl-app-heightmap ${HDR_FILES} ${SRC_FILES} "heightmapapp.qrc")

target_link_libraries(qmlgl-app-heightmap Qt5::Widgets)
target_link_libraries(qmlgl-app-heightmap qmlgl-appcommon)

if (WIN32)
  set_target_properties(qmlgl-app-heightmap PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${Qt5_DIR}/../../../bin;%PATH%")
endif()
