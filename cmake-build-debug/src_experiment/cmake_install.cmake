# Install script for directory: D:/Program Files/JetBrains/ClionProjects/DongmenDB/src_experiment

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Program Files (x86)/DongmenDB")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Program Files/JetBrains/ClionProjects/DongmenDB/cmake-build-debug/src_experiment/exp_01_stmt_parser/cmake_install.cmake")
  include("D:/Program Files/JetBrains/ClionProjects/DongmenDB/cmake-build-debug/src_experiment/exp_04_security/cmake_install.cmake")
  include("D:/Program Files/JetBrains/ClionProjects/DongmenDB/cmake-build-debug/src_experiment/exp_05_algebra_opt/cmake_install.cmake")
  include("D:/Program Files/JetBrains/ClionProjects/DongmenDB/cmake-build-debug/src_experiment/exp_07_physical_operate/cmake_install.cmake")

endif()

