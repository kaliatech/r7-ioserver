cmake_minimum_required (VERSION 3.10)
project (r7-ioserver)

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# using Clang
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

# using GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message(status "Setting GCC flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -g -Wall")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  if (MSVC)
    set(CMAKE_CXX_FLAGS "-D_SCL_SECURE_NO_WARNINGS /EHsc")
  endif (MSVC)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/ioserver")


set (VS_STARTUP_PROJECT ioserver)
