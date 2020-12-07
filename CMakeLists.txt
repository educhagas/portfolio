#######################################################
### Portfolio                                       ###
#######################################################
# Project information
cmake_minimum_required(VERSION 3.17)
project(portfolio)
set(CMAKE_CXX_STANDARD 20)

include(cmake/functions.cmake)

# Include CPM to find or download packages
# Note that CPM will try to find the packages locally before downloading them
# This avoids ODR problems
option(CPM_USE_LOCAL_PACKAGES "Try `find_package` before downloading dependencies" ON)
include(cmake/CPM.cmake)

#######################################################
### Options                                         ###
#######################################################
# Check if this a master project or a subdirectory of another project
set_master_project_vars()
# Check if we are building in DEBUG mode
set_debug_booleans()

# What to build
option(BUILD_EXAMPLES "Compile the examples" ${MASTER_PROJECT})
option(BUILD_TESTS "Compile the tests" ${MASTER_PROJECT})
option(BUILD_WITH_PEDANTIC_WARNINGS "Use pedantic warnings. This is useful for developers because many of these warnings will be in continuous integration anyway." ${DEBUG_MODE})
option(BUILD_WITH_UTF8 "Accept utf-8 in MSVC by default." ON)
if (BUILD_WITH_UTF8 AND MSVC)
    set(CMAKE_CXX_FLAGS "/utf-8")
endif ()

# Hack to check for min in Windows.h
# http://www.suodenjoki.dk/us/archive/2010/min-max.htm
include(CheckSymbolExists)
check_symbol_exists(min "Windows.h" HAVE_WINDOWS_MINMAX)
if (HAVE_WINDOWS_MINMAX)
    add_compile_definitions(NOMINMAX)
endif ()

#######################################################
### Libraries                                       ###
#######################################################
add_subdirectory(source)

#######################################################
### Examples and tests                              ###
#######################################################
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()