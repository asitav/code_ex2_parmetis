if(CMAKE_PREFIX_PATH)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
endif()

# .a priority over .so for TBD (place holder) systems
if(${CMAKE_SYSTEM_NAME} STREQUAL "TBD")
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
  if(NOT ${LIBRARY_COMPILATION_TYPE})
    set(LIBRARY_COMPILATION_TYPE STATIC)
  endif()
endif()

# "build" type
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type?" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
               "Debug"
               "Release"
               "RelWithDebInfo"
               "MinSizeRel")
endif()

set(MYPROJ_DEPENDENCIES "")

#MPI
find_package(MPI REQUIRED)
string(REGEX REPLACE "mpiexec" "mpirun" MPIRUN ${MPIEXEC})
INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${MPI_INCLUDE_DIR})
list(APPEND MYPROJ_DEPENDENCIES ${MPI_LIBRARIES})

#ParMETIS
option(WITH_ParMETIS "Link with ParMETIS?" ON)
if(WITH_ParMETIS)
  #old find_package(METIS REQUIRED)
  #old find_package(ParMETIS REQUIRED)

  set(GKLIB_PATH /usr/local/parmetis-4.0.3/metis/GKlib)
  set(METIS_PATH /usr/local/parmetis-4.0.3/metis)
  set(ParMETIS_PATH /usr/local/parmetis-4.0.3/metis)
  set(METIS_INCLUDE_DIRS /usr/local/ParMETIS_4.0.3-gnu/include)
  set(ParMETIS_INCLUDE_DIRS /usr/local/ParMETIS_4.0.3-gnu/include)
  set(METIS_LIBRARIES /usr/local/ParMETIS_4.0.3-gnu/lib/libmetis.a)
  set(ParMETIS_LIBRARIES /usr/local/ParMETIS_4.0.3-gnu/lib/libparmetis.a)

  include(${GKLIB_PATH}/GKlibSystem.cmake)

  # List of paths that the compiler will search for header files.
  # i.e., the -I equivalent
  INCLUDE_DIRECTORIES(${ParMETIS_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${METIS_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${GKLIB_PATH})
  list(APPEND MYPROJ_DEPENDENCIES ${ParMETIS_LIBRARIES} ${METIS_LIBRARIES})
endif()

