cmake_minimum_required (VERSION 3.10)
project (r7-ioserver)

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-D_SCL_SECURE_NO_WARNINGS /EHsc")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/ioserver")


set (VS_STARTUP_PROJECT ioserver)
