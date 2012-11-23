cmake_minimum_required (VERSION 2.6.2)

project (voronoi)

set (APPLICATION_NAME "Gerador de Diagrama de Voronoi")
set (APPLICATION_CODENAME "${PROJECT_NAME}")
set (APPLICATION_COPYRIGHT_YEARS "2012")
set (APPLICATION_VERSION_MAJOR 1)
set (APPLICATION_VERSION_MINOR 0)
set (APPLICATION_VERSION_PATCH 0)
set (APPLICATION_VERSION_TYPE SNAPSHOT)
set (APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}-${APPLICATION_VERSION_TYPE}")
set (APPLICATION_VENDOR_ID "com.unicentro")
set (APPLICATION_VENDOR_NAME "Unicentro")
set (APPLICATION_VENDOR_URL "unicentro.br")
set (APPLICATION_ID "${APPLICATION_VENDOR_ID}.${PROJECT_NAME}")

#
# Compiler
#

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "You may use Release and Debug types: ")
	message(STATUS "   cmake -D CMAKE_BUILD_TYPE=Release ../")
	set(CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_BUILD_TYPE)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set (CMAKE_CXX_COMPILER "clang++")
set (CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Werror -g -pipe ")
set (CMAKE_CXX_FLAGS_RELEASE "-pipe -O3 ")

#
# Debugging Options
#

set (CMAKE_VERBOSE_MAKEFILE 0) # Use 1 for debugging, 0 for release

#
# Project Output Paths
#

set (MAINFOLDER ${PROJECT_SOURCE_DIR})
set (EXECUTABLE_OUTPUT_PATH "${MAINFOLDER}/bin")
set (LIBRARY_OUTPUT_PATH "${MAINFOLDER}/lib")

#
# Project Search Paths
#

list (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}")
list (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}/tools")
list (APPEND CMAKE_PREFIX_PATH "${MAINFOLDER}/thirdparty")
set (CMAKE_MODULE_PATH "${MAINFOLDER}/tools/share/cmake")
include_directories ("${MAINFOLDER}/include")

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#
# Locate Project Prerequisites 
#

set (Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0" "1.40" "1.40.0" "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0" "1.46" "1.46.0" "1.47" "1.47.0" "1.48" "1.48.0" "1.49" "1.49.0" "1.50" "1.50.0")
find_package (Boost 1.37 COMPONENTS "date_time" "filesystem" "graph" "iostreams" "program_options" "regex" "serialization" "signals" "system" "thread" "wserialization" REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})
link_directories (${Boost_LIBRARY_DIRS})

include (FindPkgConfig)

pkg_check_modules (GLog REQUIRED libglog)
include_directories (${GLog_INCLUDE_DIRS})
link_directories (${GLog_LIBRARY_DIRS})

find_package (Gmock REQUIRED)

#
# Add Build Targets
#

add_subdirectory (src)
add_subdirectory (test)

#
# Add Install Targets
#

IF (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")
    install (DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}" DESTINATION "include")
ENDIF (EXISTS "${MAINFOLDER}/include/${PROJECT_NAME}" AND IS_DIRECTORY "${MAINFOLDER}/include/${PROJECT_NAME}")

