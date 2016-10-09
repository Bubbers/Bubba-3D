# Install script for directory: /home/roffe/workspace/bubbaroguefort/Bubba-3D/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/Input/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/Misc/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/cameras/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/collision/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/components/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/common/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/core/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/logging/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/objects/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/particle/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/shader/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/sound/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/linmath/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/glutil/cmake_install.cmake")
  include("/home/roffe/workspace/bubbaroguefort/Bubba-3D/build/src/userInterface/cmake_install.cmake")

endif()

