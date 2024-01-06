
add_subdirectory(tetrahedron)

set(QMLGL_BUILD_APP_HEIGHTMAP FALSE CACHE BOOL "Build the 'heightmap' application")
if(QMLGL_BUILD_APP_HEIGHTMAP)
  add_subdirectory(heightmap)
endif()

set(QMLGL_BUILD_APP_ASSIMP FALSE CACHE BOOL "Build the Assimp application")
if(QMLGL_BUILD_APP_ASSIMP)
  add_subdirectory(assimp)
endif()
