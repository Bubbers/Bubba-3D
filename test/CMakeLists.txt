cmake_minimum_required(VERSION 2.8)

set(TEST_NAME Bubba3DTest)

project(Testing)

if(USE_CPP11)
    ## We can't turn this on by default, since it breaks on travis
    message(STATUS "Enabling C++11")
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

# configure the executable

add_executable(${TEST_NAME} test.cpp ${files_SOURCE})

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
# FIND OPENAL
#########################################################
find_package(OPENAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})
target_link_libraries(${TEST_NAME}  ${OPENAL_LIBRARY})

if(NOT OPENAL_FOUND)
    message(ERROR " OPENAL not found!")
endif(NOT OPENAL_FOUND)

#########################################################
# FIND ALUT
#########################################################
find_package(ALUT REQUIRED)
include_directories(${ALUT_INCLUDE_DIR})
include_directories(${ALUT_WORKAROUND_INCLUDE_DIRS})
target_link_libraries(${TEST_NAME}  ${ALUT_LIBRARY})


if(NOT ALUT_FOUND)
    message(ERROR " ALUT not found!")
endif(NOT ALUT_FOUND)


#########################################################
# FIND OGGVORBIS
#########################################################
find_package(OGGVORBIS REQUIRED)
include_directories(${VORBIS_INCLUDE_DIR})
include_directories(${OGG_INCLUDE_DIR})
target_link_libraries(${TEST_NAME}  ${OGG_LIBRARY})
target_link_libraries(${TEST_NAME}  ${VORBIS_LIBRARY})
target_link_libraries(${TEST_NAME}  ${VORBISFILE_LIBRARY})
target_link_libraries(${TEST_NAME}  ${ALUT_LIBRARY})



if(NOT OGGVORBIS_FOUND)
    message(ERROR " OGGVORBIS not found!")
endif(NOT OGGVORBIS_FOUND)



#########################################################
# FIND FREEIMAGE
#########################################################
find_package(FREEIMAGE REQUIRED)
include_directories(${FREEIMAGE_INCLUDE_PATH})
target_link_libraries(${TEST_NAME}  ${FREEIMAGE_LIBRARY})
target_link_libraries(${TEST_NAME}  ${FREEIMAGE_LIBRARIES})

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
target_link_libraries(${TEST_NAME}  ${ASSIMP_LIBRARY})
target_link_libraries(${TEST_NAME}  ${ASSIMP_LIBRARIES})
add_definitions(${ASSIMP_DEFINITIONS})


if(NOT ASSIMP_FOUND)
    message(ERROR " ASSIMP not found!")
endif(NOT ASSIMP_FOUND)

# configure unit tests via CTest
enable_testing()
add_test(NAME trolo COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} ${ASSIMP_LIBRARY})
