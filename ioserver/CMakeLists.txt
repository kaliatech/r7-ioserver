#set(CMAKE_VERBOSE_MAKEFILE ON)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "include/ioserver/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/generated/ioserver/TutorialConfig.h"
  )

include_directories("${PROJECT_BINARY_DIR}/generated"
                    "include"
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#target_link_libraries(Tutorial Threads::Threads)

#file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cxx")

SET(HEADERS_CIVETWEB
    "include/civetweb/CivetServer.h"
    "include/civetweb/civetweb.h"
)

SET(HEADERS_MISC
    "include/nlohmann-json/json.hpp"
    "include/pystring/pystring.h"
    "include/sqlite3/sqlite3.h"
)


#SET(HEADERS_IOSERVER
#    "include/ioserver/IoController.h"
#    "include/ioserver/IoServerConfig.h"
#    "include/ioserver/IoServerContext.h"
#    "include/ioserver/IoServerWebUtils.h"
#    "include/ioserver/ClientDataHandler.h"
#    "include/ioserver/ServoMoveHandler.h"
#    "include/ioserver/serialport/AsyncSerialEvent.h"
#    "include/ioserver/serialport/AsyncSerialPort.h"
#)

set(SOURCES_CIVETWEB
	"src/civetweb/civetweb.c"
	"src/civetweb/CivetServer.cpp"
	"src/civetweb/handle_form.inl"
	"src/civetweb/md5.inl"
)

file(GLOB_RECURSE HEADERS_IOSERVER "*.h")
file(GLOB_RECURSE SOURCES_IOSERVER "*.cpp" "*.hpp")

#set(SOURCES_IOSERVER
#        "src/ioserver/IoController.cpp"
#	"src/ioserver/IoServerConfig.cpp"
#        "src/ioserver/IoServerContext.cpp"
#	"src/ioserver/IoServer.cpp"
#	"src/ioserver/ClientDataHandler.cpp"
#	"src/ioserver/ServoMoveHandler.cpp"
#        "src/ioserver/serialport/AsyncSerialEvent.cpp"
#        "src/ioserver/serialport/AsyncSerialPort.cpp"
#)

set(SOURCES_MISC
  "src/pystring/pystring.cpp"
  "src/sqlite3/sqlite3.c"
)


# add_definitions(-D_TIMESPEC_DEFINED)

#file(COPY "data" DESTINATION "${PROJECT_BINARY_DIR}/data")
 
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/data/config.json" "${CMAKE_CURRENT_BINARY_DIR}/${CONFIGURATION}/config.json" COPYONLY)
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/data/config.json" "${RUNTIME_OUTPUT_DIRECTORY}/${CONFIGURATION}/config.json" COPYONLY)


# headers folder for visual studio
#source_group("Header Files/civetweb" FILES ${HEADERS_CIVETWEB})
#source_group("Header Files/ioserver" FILES ${HEADERS_IOSERVER})
#source_group(TREE "../ioserver/include" PREFIX "includes" FILES ${HEADERS_CIVETWEB} ${HEADERS_IOSERVER} ${HEADERS_MISC})
#source_group(TREE "../ioserver/src" PREFIX "src" FILES ${SOURCES_CIVETWEB} ${SOURCES_IOSERVER} ${SOURCES_MISC})


set(Boost_USE_STATIC_LIBS        ON) # only find static libs
#set(Boost_USE_DEBUG_LIBS         ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
find_package(Boost 1.65.1 REQUIRED COMPONENTS thread system regex signals filesystem program_options)
# if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
# endif(Boost_FOUND)

# https://stackoverflow.com/a/40217291/123378
if (WIN32)
    macro(get_WIN32_WINNT version)
        if (CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            # Check for Windows 10, b/c we'll need to convert to hex 'A'.
            if ("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif ("${verMajor}" MATCHES "10")
            # Remove all remaining '.' characters.
            string(REPLACE "." "" ver ${ver})
            # Prepend each digit with a zero.
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif(CMAKE_SYSTEM_VERSION)
    endmacro(get_WIN32_WINNT)

    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
endif(WIN32)

# Add preprocessor flags
add_definitions(-DELPP_THREAD_SAFE)
add_definitions(-DELPP_STL_LOGGING) # TODO: Worthwhile?
add_definitions(-DELPP_NO_DEFAULT_LOG_FILE)
add_definitions(-DELPP_NO_LOG_TO_FILE)

# headers included only so that they appear in visual studio project
add_executable(ioserver
	${HEADERS_CIVETWEB}
	${SOURCES_CIVETWEB}
	${HEADERS_IOSERVER}
	${SOURCES_IOSERVER}
	${HEADERS_MISC}
        ${SOURCES_MISC}
)
#target_compile_options(Tutorial PUBLIC -DHAVE_STRUCT_TIMESPEC)
target_link_libraries(ioserver ${Boost_LIBRARIES} Threads::Threads ${CMAKE_DL_LIBS})
#target_link_libraries(anyExecutable Boost::program_options)

#add_custom_command(TARGET IoServer POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${CMAKE_CURRENT_SOURCE_DIR}/data"
#        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/data")

add_custom_command(TARGET ioserver POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "${CMAKE_CURRENT_BINARY_DIR}/data")


if (MSVC)

endif (MSVC)

