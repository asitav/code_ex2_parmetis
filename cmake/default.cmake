# Set default options for some compilers

# GNU compiler
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_C_FLAGS_INIT "-std=c11 -Wall -Wextra -Wno-unused-parameter")
  set(CMAKE_C_FLAGS_DEBUG_INIT  "-O0 -g")
  set(CMAKE_C_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS_INIT "-std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-long-long")
  set(CMAKE_CXX_FLAGS_DEBUG_INIT  "-O0 -g")
  set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
endif()

# Intel compiler
if("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
  set(CMAKE_C_FLAGS_INIT "-std=c11 -Wall -pedantic -wd981 -wd383 -wd869 -g -traceback")
  set(CMAKE_C_FLAGS_DEBUG_INIT  "-O0")
  set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  set(CMAKE_CXX_FLAGS_INIT "-std=c++11 -Wall -pedantic -wd981 -wd383 -wd869 -g -traceback")
  set(CMAKE_C_FLAGS_DEBUG_INIT  "-O0")
  set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
endif()

# CLANG compiler
if("${CMAKE_C_COMPILER_ID}" STREQUAL "CLANG")
  set(CMAKE_C_FLAGS_INIT "-std=c11 -Wall -Wextra -Wunused-parameter -Wunused-but-set-parameter")
  set(CMAKE_C_FLAGS_DEBUG_INIT  "-O0 -g -traceback")
  set(CMAKE_C_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG")
  set(CMAKE_CXX_FLAGS_INIT "-std=c++11 -Wall -Wextra -Wunused-parameter -Wno-long-long")
  set(CMAKE_CXX_FLAGS_DEBUG_INIT  "-O0 -g -traceback")
  set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
endif()
