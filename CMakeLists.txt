cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(Bubba-test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


#file(GLOB files_SOURCE  )
file(GLOB files_SOURCE "src/*.cpp" "includes/linmath/*.cpp" "includes/glutil/*.cpp" "src/logging/*.cpp"  "src/Misc/*.cpp" "src/effects/*.cpp" "src/cameras/*.cpp" "src/core/*.cpp" "src/objects/*.cpp" "src/collision/*.cpp")

add_executable(testz src/main.cpp ${files_SOURCE})

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
# FIND DEVIL
#########################################################
find_package(DevIL REQUIRED)
include_directories(${IL_INCLUDE_DIR})
link_directories(${IL_LIBRARIES})
link_directories(${ILU_LIBRARIES})
link_directories(${ILUT_LIBRARIES})

add_definitions(${IL_DEFINITIONS})
if(NOT IL_FOUND)
    message(ERROR " DevIL not found!")
endif(NOT IL_FOUND)


#########################################################
# FIND ASSIMP
#########################################################
find_package(ASSIMP REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIR})
target_link_libraries(testz ${ASSIMP_LIBRARY})

if(NOT ASSIMP_FOUND)
    message(ERROR " ASSIMP not found!")
endif(NOT ASSIMP_FOUND)

#########################################################
# Include Files
#########################################################

include_directories("${PROJECT_SOURCE_DIR}/includes")
include_directories("${PROJECT_SOURCE_DIR}/includes/linmath")
message("${PROJECT_SOURCE_DIR}/src")



########################################################
# Linking & stuff
###################################################### ###

  # create the program "test"
  target_link_libraries(testz ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} ${ASSIMP_LIBRARY})
