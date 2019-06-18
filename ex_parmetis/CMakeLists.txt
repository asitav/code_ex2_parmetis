cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)

include_directories(${CMAKE_SOURCE_DIR}/include
                    ${CMAKE_BINARY_DIR}/include
                   )


set(PROJECTNAME parmetis_tests)
project(${PROJECTNAME})

if(NOT LIB_FINDING_INCLUDED) # building manually
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake)
  include("${CMAKE_SOURCE_DIR}/../../cmake/findlibs.cmake")
  find_package(MYPROJ)
  include_directories(${MYPROJ_INCLUDE_DIRS})
  set(GCOV_LIBS "gcov")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
endif()

message("In ex_parmetis -- for testing")
message("MYPROJ_DEPENDENCIES: ${MYPROJ_DEPENDENCIES}")

add_executable(parmetis_test parmetis_test.cpp)
# Link to libparmetis.
target_link_libraries(parmetis_test ${MYPROJ_DEPENDENCIES})


#install instruction
set (CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS parmetis_test
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
