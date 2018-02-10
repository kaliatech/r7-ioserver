
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
)


SET(HEADERS_IOSERVER
    "include/ioserver/IoServerConfig.h"
    "include/ioserver/IoServerWebUtils.h"
	"include/ioserver/WebHandlerClientData.h"
)

set(SOURCES_CIVETWEB
	"src/civetweb/civetweb.c"
	"src/civetweb/CivetServer.cpp"
	"src/civetweb/handle_form.inl"
	"src/civetweb/md5.inl"
)

set(SOURCES_IOSERVER
	"src/ioserver/IoServerConfig.cpp"
	"src/ioserver/IoServer.cpp"
	"src/ioserver/WebHandlerClientData.cpp"
)

# add_definitions(-D_TIMESPEC_DEFINED)

#file(COPY "data" DESTINATION "${PROJECT_BINARY_DIR}/data")
 
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/data/config.json" "${CMAKE_CURRENT_BINARY_DIR}/${CONFIGURATION}/config.json" COPYONLY)
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/data/config.json" "${RUNTIME_OUTPUT_DIRECTORY}/${CONFIGURATION}/config.json" COPYONLY)


# headers folder for visual studio
#source_group("Header Files/civetweb" FILES ${HEADERS_CIVETWEB})
#source_group("Header Files/ioserver" FILES ${HEADERS_IOSERVER})
source_group(TREE "../ioserver/include" PREFIX "includes" FILES ${HEADERS_CIVETWEB} ${HEADERS_IOSERVER} ${HEADERS_MISC})
source_group(TREE "../ioserver/src" PREFIX "src" FILES ${SOURCES_CIVETWEB} ${SOURCES_IOSERVER})

# headers included only so that they appear in visual studio project
add_executable(IoServer
	${HEADERS_CIVETWEB}
	${SOURCES_CIVETWEB}
	${HEADERS_IOSERVER}
	${SOURCES_IOSERVER}
	${HEADERS_MISC}
)
#target_compile_options(Tutorial PUBLIC -DHAVE_STRUCT_TIMESPEC)


#add_custom_command(TARGET IoServer POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${CMAKE_CURRENT_SOURCE_DIR}/data"
#        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/data")

add_custom_command(TARGET IoServer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "${CMAKE_CURRENT_BINARY_DIR}/data")


if (MSVC)

endif (MSVC)
