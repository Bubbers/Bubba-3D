cmake_minimum_required(VERSION 3.0)
# Project Name
PROJECT(Bubba-3D)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

if(UNIX)
    set(CMAKE_CXX_FLAGS "-g")
endif(UNIX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")
endif()


file(GLOB BUBBA3D_FILES_SOURCE
        "${PROJECT_SOURCE_DIR}/includes/linmath/*.cpp"
        "${PROJECT_SOURCE_DIR}/includes/glutil/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/logging/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Misc/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/cameras/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/core/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/objects/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/collision/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/particle/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/components/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Input/*.cpp"
        "${PROJECT_SOURCE_DIR}/shaders/*")


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
add_definitions(${ASSIMP_DEFINITIONS})


if(NOT ASSIMP_FOUND)
    message(ERROR " ASSIMP not found!")
endif(NOT ASSIMP_FOUND)


#########################################################
# FIND SFML AUDIO
#########################################################
find_package(SFML 2 COMPONENTS audio window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})


if(NOT SFML_FOUND)
    message(ERROR " SFML not found!")
endif(NOT SFML_FOUND)

#########################################################
# Testing
#########################################################
add_subdirectory (src)
include(CTest)
add_subdirectory(test)