cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)

# this line must be set BEFORE "project()" or it will be ignored!
set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_SOURCE_DIR}/cmake/default.cmake")

project(MYPROJ C CXX)

if(NOT LIB_FINDING_INCLUDED)
  set(LIB_FINDING_INCLUDED TRUE)
endif()

if(CMAKE_PREFIX_PATH)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/../install" CACHE PATH "Install path?" FORCE)
  set(MYPROJ ${CMAKE_INSTALL_PREFIX})
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/FindModules)
include(macros)
include(default)
include(findlibs) # external libraries

# "build" type
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type?" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
               "Debug"
               "Release"
               "RelWithDebInfo"
               "MinSizeRel")
endif()

# STATIC vs. SHARED
set(BUILD_LIBRARY_TYPE "SHARED" CACHE STRING "STATIC vs. DYNAMIC/SHARED library?")
set_property(CACHE BUILD_LIBRARY_TYPE PROPERTY STRINGS "SHARED;STATIC")

# program
add_subdirectory(ex_metis)
add_subdirectory(ex_parmetis)
add_subdirectory(ex_parmetisIO)
add_subdirectory(ex_partmesh)

configure_file(cmake/MYPROJ-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/MYPROJ-config.cmake)
