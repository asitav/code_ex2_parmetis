###
#
# @copyright (c) 2009-2014 The University of Tennessee and The University
#                          of Tennessee Research Foundation.
#                          All rights reserved.
# @copyright (c) 2012-2014 Inria. All rights reserved.
# @copyright (c) 2012-2014 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria, Univ. Bordeaux. All rights reserved.
#
###
#
# - Find PARMETIS include dirs and libraries
# Use this module by invoking find_package with the form:
#  find_package(PARMETIS
#               [REQUIRED]             # Fail with error if parmetis is not found
#              )
#
# This module finds headers and parmetis library.
# Results are reported in variables:
#  PARMETIS_FOUND           - True if headers and requested libraries were found
#  PARMETIS_INCLUDE_DIRS    - parmetis include directories
#  PARMETIS_LIBRARY_DIRS    - Link directories for parmetis libraries
#  PARMETIS_LIBRARIES       - parmetis component libraries to be linked
#
# The user can give specific paths where to find the libraries adding cmake
# options at configure (ex: cmake path/to/project -DPARMETIS_DIR=path/to/parmetis):
#  PARMETIS_DIR             - Where to find the base directory of parmetis
#  PARMETIS_INCDIR          - Where to find the header files
#  PARMETIS_LIBDIR          - Where to find the library files
# The module can also look for the following environment variables if paths
# are not given as cmake variable: PARMETIS_DIR, PARMETIS_INCDIR, PARMETIS_LIBDIR

#=============================================================================
# Copyright 2012-2013 Inria
# Copyright 2012-2013 Emmanuel Agullo
# Copyright 2012-2013 Mathieu Faverge
# Copyright 2012      Cedric Castagnede
# Copyright 2013      Florent Pruvost
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file MORSE-Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of Morse, substitute the full
#  License text for the above reference.)

if (NOT PARMETIS_FOUND)
  set(PARMETIS_DIR "" CACHE PATH "Installation directory of PARMETIS library")
  if (NOT PARMETIS_FIND_QUIETLY)
    message(STATUS "A cache variable, namely PARMETIS_DIR, has been set to
    specify the install directory of PARMETIS")
  endif()
endif()

# Looking for include
# -------------------

# Add system include paths to search include
# ------------------------------------------
unset(_inc_env)
set(ENV_PARMETIS_DIR "$ENV{PARMETIS_DIR}")
set(ENV_PARMETIS_INCDIR "$ENV{PARMETIS_INCDIR}")
if(ENV_PARMETIS_INCDIR)
  list(APPEND _inc_env "${ENV_PARMETIS_INCDIR}")
elseif(ENV_PARMETIS_DIR)
  list(APPEND _inc_env "${ENV_PARMETIS_DIR}")
  list(APPEND _inc_env "${ENV_PARMETIS_DIR}/include")
  list(APPEND _inc_env "${ENV_PARMETIS_DIR}/include/parmetis")
else()
  if(WIN32)
    string(REPLACE ":" ";" _inc_env "$ENV{INCLUDE}")
  else()
    string(REPLACE ":" ";" _path_env "$ENV{INCLUDE}")
    list(APPEND _inc_env "${_path_env}")
    string(REPLACE ":" ";" _path_env "$ENV{C_INCLUDE_PATH}")
    list(APPEND _inc_env "${_path_env}")
    string(REPLACE ":" ";" _path_env "$ENV{CPATH}")
    list(APPEND _inc_env "${_path_env}")
    string(REPLACE ":" ";" _path_env "$ENV{INCLUDE_PATH}")
    list(APPEND _inc_env "${_path_env}")
  endif()
endif()
list(APPEND _inc_env "${CMAKE_PLATFORM_IMPLICIT_INCLUDE_DIRECTORIES}")
list(APPEND _inc_env "${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}")
list(REMOVE_DUPLICATES _inc_env)


# Try to find the parmetis header in the given paths
# -------------------------------------------------
# call cmake macro to find the header path
if(PARMETIS_INCDIR)
  set(PARMETIS_parmetis.h_DIRS "PARMETIS_parmetis.h_DIRS-NOTFOUND")
  find_path(PARMETIS_parmetis.h_DIRS
    NAMES parmetis.h
    HINTS ${PARMETIS_INCDIR})
else()
  if(PARMETIS_DIR)
    set(PARMETIS_parmetis.h_DIRS "PARMETIS_parmetis.h_DIRS-NOTFOUND")
    find_path(PARMETIS_parmetis.h_DIRS
      NAMES parmetis.h
      HINTS ${PARMETIS_DIR}
      PATH_SUFFIXES "include" "include/parmetis")
  else()
    set(PARMETIS_parmetis.h_DIRS "PARMETIS_parmetis.h_DIRS-NOTFOUND")
    find_path(PARMETIS_parmetis.h_DIRS
      NAMES parmetis.h
      HINTS ${_inc_env})
  endif()
endif()
mark_as_advanced(PARMETIS_parmetis.h_DIRS)


# If found, add path to cmake variable
# ------------------------------------
if (PARMETIS_parmetis.h_DIRS)
  set(PARMETIS_INCLUDE_DIRS "${PARMETIS_parmetis.h_DIRS}")
else ()
  set(PARMETIS_INCLUDE_DIRS "PARMETIS_INCLUDE_DIRS-NOTFOUND")
  if(NOT PARMETIS_FIND_QUIETLY)
    message(STATUS "Looking for parmetis -- parmetis.h not found")
  endif()
endif()


# Looking for lib
# ---------------

# Add system library paths to search lib
# --------------------------------------
unset(_lib_env)
set(ENV_PARMETIS_LIBDIR "$ENV{PARMETIS_LIBDIR}")
if(ENV_PARMETIS_LIBDIR)
  list(APPEND _lib_env "${ENV_PARMETIS_LIBDIR}")
elseif(ENV_PARMETIS_DIR)
  list(APPEND _lib_env "${ENV_PARMETIS_DIR}")
  list(APPEND _lib_env "${ENV_PARMETIS_DIR}/lib")
else()
  if(WIN32)
    string(REPLACE ":" ";" _lib_env "$ENV{LIB}")
  else()
    if(APPLE)
      string(REPLACE ":" ";" _lib_env "$ENV{DYLD_LIBRARY_PATH}")
    else()
      string(REPLACE ":" ";" _lib_env "$ENV{LD_LIBRARY_PATH}")
    endif()
    list(APPEND _lib_env "${CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES}")
    list(APPEND _lib_env "${CMAKE_C_IMPLICIT_LINK_DIRECTORIES}")
  endif()
endif()
list(REMOVE_DUPLICATES _lib_env)

# Try to find the parmetis lib in the given paths
# ----------------------------------------------
# call cmake macro to find the lib path
if(PARMETIS_LIBDIR)
  set(PARMETIS_parmetis_LIBRARY "PARMETIS_parmetis_LIBRARY-NOTFOUND")
  find_library(PARMETIS_parmetis_LIBRARY
    NAMES parmetis
    HINTS ${PARMETIS_LIBDIR})
else()
  if(PARMETIS_DIR)
    set(PARMETIS_parmetis_LIBRARY "PARMETIS_parmetis_LIBRARY-NOTFOUND")
    find_library(PARMETIS_parmetis_LIBRARY
      NAMES parmetis
      HINTS ${PARMETIS_DIR}
      PATH_SUFFIXES lib lib32 lib64)
  else()
    set(PARMETIS_parmetis_LIBRARY "PARMETIS_parmetis_LIBRARY-NOTFOUND")
    find_library(PARMETIS_parmetis_LIBRARY
      NAMES parmetis
      HINTS ${_lib_env})
  endif()
endif()
mark_as_advanced(PARMETIS_parmetis_LIBRARY)


# If found, add path to cmake variable
# ------------------------------------
if (PARMETIS_parmetis_LIBRARY)
  get_filename_component(parmetis_lib_path "${PARMETIS_parmetis_LIBRARY}" PATH)
  # set cmake variables
  set(PARMETIS_LIBRARIES    "${PARMETIS_parmetis_LIBRARY}")
  set(PARMETIS_LIBRARY_DIRS "${parmetis_lib_path}")
else ()
  set(PARMETIS_LIBRARIES    "PARMETIS_LIBRARIES-NOTFOUND")
  set(PARMETIS_LIBRARY_DIRS "PARMETIS_LIBRARY_DIRS-NOTFOUND")
  if(NOT PARMETIS_FIND_QUIETLY)
    message(STATUS "Looking for parmetis -- lib parmetis not found")
  endif()
endif ()

# check a function to validate the find
if(PARMETIS_LIBRARIES)

  set(REQUIRED_INCDIRS)
  set(REQUIRED_LIBDIRS)
  set(REQUIRED_LIBS)

  # PARMETIS
  if (PARMETIS_INCLUDE_DIRS)
    set(REQUIRED_INCDIRS  "${PARMETIS_INCLUDE_DIRS}")
  endif()
  if (PARMETIS_LIBRARY_DIRS)
    set(REQUIRED_LIBDIRS "${PARMETIS_LIBRARY_DIRS}")
  endif()
  set(REQUIRED_LIBS "${PARMETIS_LIBRARIES}")
  # m
  find_library(M_LIBRARY NAMES m)
  mark_as_advanced(M_LIBRARY)
  if(M_LIBRARY)
    list(APPEND REQUIRED_LIBS "-lm")
  endif()

  # set required libraries for link
  set(CMAKE_REQUIRED_INCLUDES "${REQUIRED_INCDIRS}")
  set(CMAKE_REQUIRED_LIBRARIES)
  foreach(lib_dir ${REQUIRED_LIBDIRS})
    list(APPEND CMAKE_REQUIRED_LIBRARIES "-L${lib_dir}")
  endforeach()
  list(APPEND CMAKE_REQUIRED_LIBRARIES "${REQUIRED_LIBS}")
  string(REGEX REPLACE "^ -" "-" CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES}")

  # test link
  unset(PARMETIS_WORKS CACHE)
  include(CheckFunctionExists)
  check_function_exists(ParMETIS_V3_NodeND METIS_WORKS)
  mark_as_advanced(PARMETIS_WORKS)

  if(NOT PARMETIS_WORKS)
    if(NOT PARMETIS_FIND_QUIETLY)
      message(STATUS "Looking for ParMETIS : test of ParMETIS_V3_NodeND with ParMETIS library fails")
      message(STATUS "CMAKE_REQUIRED_LIBRARIES: ${CMAKE_REQUIRED_LIBRARIES}")
      message(STATUS "CMAKE_REQUIRED_INCLUDES: ${CMAKE_REQUIRED_INCLUDES}")
      message(STATUS "Check in CMakeFiles/CMakeError.log to figure out why it fails")
    endif()
  endif()
  set(CMAKE_REQUIRED_INCLUDES)
  set(CMAKE_REQUIRED_FLAGS)
  set(CMAKE_REQUIRED_LIBRARIES)
endif(PARMETIS_LIBRARIES)

if (PARMETIS_LIBRARIES)
  list(GET PARMETIS_LIBRARIES 0 first_lib)
  get_filename_component(first_lib_path "${first_lib}" PATH)
  if (${first_lib_path} MATCHES "/lib(32|64)?$")
    string(REGEX REPLACE "/lib(32|64)?$" "" not_cached_dir "${first_lib_path}")
    set(PARMETIS_DIR_FOUND "${not_cached_dir}" CACHE PATH "Installation
    directory of PARMETIS library" FORCE)
  else()
    set(PARMETIS_DIR_FOUND "${first_lib_path}" CACHE PATH "Installation
    directory of PARMETIS library" FORCE)
  endif()
endif()
mark_as_advanced(PARMETIS_DIR)
mark_as_advanced(PARMETIS_DIR_FOUND)

# check that PARMETIS has been found
# ---------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PARMETIS DEFAULT_MSG
  PARMETIS_LIBRARIES)
#
# TODO: Add possibility to check for specific functions in the library
#
