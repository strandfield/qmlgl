
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(qmlgl-app-tetrahedron ${HDR_FILES} ${SRC_FILES} "tetrahedronapp.qrc")

target_link_libraries(qmlgl-app-tetrahedron qmlgl-appcommon)

if (WIN32)
  set_target_properties(qmlgl-app-tetrahedron PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${Qt5_DIR}/../../../bin;%PATH%")
endif()
