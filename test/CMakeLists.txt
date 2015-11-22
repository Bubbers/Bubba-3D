cmake_minimum_required(VERSION 2.8)

project(Testing)

if(USE_CPP11)
    ## We can't turn this on by default, since it breaks on travis
    message(STATUS "Enabling C++11")
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

# define the sources of the self test
set(SOURCES
        test.cpp
        )

# configure the executable
add_executable(TestExec ${SOURCES})

# configure unit tests via CTest
enable_testing()
add_test(NAME trolo COMMAND TestExec)