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
# Testing
#########################################################
add_subdirectory (src)
include(CTest)
add_subdirectory(test)