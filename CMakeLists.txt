cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(Bubba-test)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
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
include_directories(${GLEW_INCLUDE_PATH})
link_directories(${GLEW_LIBRARY})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)

#########################################################
# FIND DEVIL
#########################################################
find_package(DevIL REQUIRED)
include_directories(${IL_INCLUDE_DIR})
link_directories(${IL_LIBRARY})
link_directories(${ILU_LIBRARY})
link_directories(${ILUT_LIBRARY})

add_definitions(${IL_DEFINITIONS})
if(NOT IL_FOUND)
    message(ERROR " DevIL not found!")
endif(NOT IL_FOUND)

#########################################################
# Include Files
#########################################################
file(GLOB files_SOURCE "src/objects/Scene.cpp" "src/logging/Logger.cpp" "includes/linmath/*.cpp" "includes/glutil/glutil.cpp" )


add_executable(test src/test.cpp ${files_SOURCE})
include_directories("${PROJECT_SOURCE_DIR}/includes")
include_directories("${PROJECT_SOURCE_DIR}/includes/linmath")


########################################################
# Linking & stuff
###################################################### ###

  # create the program "test"
  target_link_libraries(test ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARY})
