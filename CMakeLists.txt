cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(Bubba-3D)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if(UNIX)
    #set(ASSIMP_INCLUDE_DIR "$HOME/assimp/assimp/include")
    #set(${ASSIMP_LIBRARY}  "$HOME/assimp/assimp/lib/libassimp.so.3.1.1")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
    set(CMAKE_CXX_FLAGS "-g")
endif(UNIX)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")
endif()
#########################################################
# ADD SOURCE FILES TO EXECUTABLE
#########################################################
file(GLOB files_SOURCE "src/*.cpp" "includes/linmath/*.cpp" "includes/glutil/*.cpp" "src/logging/*.cpp" "src/util/*.cpp" "src/Misc/*.cpp" "src/effects/*.cpp" "src/cameras/*.cpp" "src/core/*.cpp" "src/objects/*.cpp" "src/collision/*.cpp" "shaders/*")
add_executable(testz src/main.cpp ${files_SOURCE} includes/ResourceManager.h src/Misc/ResourceManager.cpp includes/Texture.h src/objects/Texture.cpp includes/DestructorUtils.h)


#########################################################
# STRUCTURE MSVS
#########################################################
source_group(Logging	FILES src/logging/Logger.cpp)
source_group(Cameras	FILES src/cameras/Camera.cpp src/cameras/PerspectiveCamera.cpp)
source_group(Objects	FILES src/objects/Mesh.cpp src/objects/Scene.cpp src/objects/Skybox.cpp src/objects/Triangle.cpp)
source_group(Collision	FILES src/collision/Collider.cpp src/collision/Octree.cpp)
source_group(Core		FILES src/core/Renderer.cpp)
source_group(Misc		FILES src/Misc/CubeMapTexture.cpp includes/glutil/glutil.cpp)
source_group(Math		FILES includes/linmath/float2.cpp includes/linmath/float3.cpp includes/linmath/float3x3.cpp includes/linmath/float4.cpp includes/linmath/float4x4.cpp includes/linmath/int2.cpp includes/linmath/int3.cpp includes/linmath/int4.cpp includes/linmath/Quaternion.cpp)
source_group(Shaders    FILES shaders/simple.frag shaders/simple.vert shaders/skybox.frag shaders/skybox.vert shaders/vertical_blur.frag shaders/horizontal_blur.frag shaders/cutoff.frag shaders/postFx.frag shaders/postFx.vert shaders/shadowMap.frag shaders/shadowMap.vert)


#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND GLEW
#########################################################
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
link_directories(${GLEW_LIBRARY})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)


#########################################################
# FIND FREEIMAGE
#########################################################
find_package(FREEIMAGE REQUIRED)
include_directories(${FREEIMAGE_INCLUDE_PATH})
target_link_libraries(testz ${FREEIMAGE_LIBRARY})
target_link_libraries(testz ${FREEIMAGE_LIBRARIES})

if(NOT FREEIMAGE_FOUND)
    message(ERROR " FreeImage not found!")
endif(NOT FREEIMAGE_FOUND)

#########################################################
# FIND ASSIMP
#########################################################
find_package(ASSIMP REQUIRED)
if(ASSIMP_INCLUDE_DIR) 
	include_directories(${ASSIMP_INCLUDE_DIR})
endif()
#find_library(ASSIMP_LIBRARY NAMES libassimp.so)
target_link_libraries(testz ${ASSIMP_LIBRARY})
target_link_libraries(testz ${ASSIMP_LIBRARIES})
add_definitions(${ASSIMP_DEFINITIONS})


if(NOT ASSIMP_FOUND)
    message(ERROR " ASSIMP not found!")
endif(NOT ASSIMP_FOUND)

#########################################################
# Include Files
#########################################################

include_directories("${PROJECT_SOURCE_DIR}/includes")
include_directories("${PROJECT_SOURCE_DIR}/src/util")
include_directories("${PROJECT_SOURCE_DIR}/includes/linmath")



########################################################
# Linking & stuff
###################################################### ###

target_link_libraries(testz ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} ${ASSIMP_LIBRARY})