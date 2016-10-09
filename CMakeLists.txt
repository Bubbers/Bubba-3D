cmake_minimum_required(VERSION 3.0)
# Project Name
PROJECT(Bubba-3D)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")
endif()


#########################################################
# Testing
#########################################################
include_directories(src)
add_subdirectory (src)
include(CTest)
add_subdirectory(test)


#########################################################
# Docs
#########################################################
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)"
                           ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
         message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${PROJECT_SOURCE_DIR}/doxygen.conf.in)
    set(doxyfile ${PROJECT_SOURCE_DIR}/doxygen.conf)
    set(GENERATE_HTML YES)
    set(GENERATE_LATEX NO)

    configure_file(${doxyfile_in} ${doxyfile})

    message("Doxygen build started.")

    add_custom_target(doc ALL
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                      WORKING_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)

    #    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION     share/doc)
endif()
