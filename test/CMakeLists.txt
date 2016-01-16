cmake_minimum_required(VERSION 3.0)
set(TEST_NAME Bubba3DTest)


if(USE_CPP11)
    ## We can't turn this on by default, since it breaks on travis
    message(STATUS "Enabling C++11")
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

# configure the executable
file(GLOB BUBBA3D_FILES_SOURCE_TEST
        "${PROJECT_SOURCE_DIR}/includes/linmath/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/objects/Triangle.cpp"
        "${PROJECT_SOURCE_DIR}/src/objects/*.cpp"
        "${PROJECT_SOURCE_DIR}/includes/glutil/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/logging/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Misc/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/cameras/*.cpp"
        #"${PROJECT_SOURCE_DIR}/src/core/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/objects/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/collision/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/particle/*.cpp"
        #"${PROJECT_SOURCE_DIR}/src/components/*.cpp"
        #"${PROJECT_SOURCE_DIR}/src/Input/*.cpp"
        "${PROJECT_SOURCE_DIR}/shaders/*"
        )


add_executable(${TEST_NAME} test.cpp ${BUBBA3D_FILES_SOURCE_TEST})

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


target_include_directories (${TEST_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes")
target_include_directories (${TEST_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes/linmath")
target_include_directories (${TEST_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes/rapidjson")

# configure unit tests via CTest
enable_testing()
add_test(NAME trolo COMMAND ${TEST_NAME})



target_link_libraries(${TEST_NAME}  ${FREEIMAGE_LIBRARY})
target_link_libraries(${TEST_NAME}  ${FREEIMAGE_LIBRARIES})
target_link_libraries(${TEST_NAME}  ${ASSIMP_LIBRARY})
target_link_libraries(${TEST_NAME}  ${ASSIMP_LIBRARIES})

target_link_libraries(${TEST_NAME}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} ${ASSIMP_LIBRARY})

