
find_package(Assimp REQUIRED)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*)
file(GLOB_RECURSE QML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml)

add_executable(qmlgl-app-assimp ${HDR_FILES} ${SRC_FILES} "assimpapp.qrc" ${SHADER_FILES} ${QML_FILES})

target_link_libraries(qmlgl-app-assimp assimp::assimp)
target_link_libraries(qmlgl-app-assimp qmlgl-appcommon)

if (WIN32)
  set_target_properties(qmlgl-app-assimp PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${Qt5_DIR}/../../../bin;${ASSIMP_ROOT_DIR}/bin;%PATH%")
endif()
