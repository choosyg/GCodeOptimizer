cmake_minimum_required (VERSION 3.16.0)
project( GCodeOptimizer )

set( GCodeOptimizer_VERSION_MAJOR 1 )
set( GCodeOptimizer_VERSION_MINOR 0 )
set( GCodeOptimizer_VERSION_PATCH 0 )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

set(CMAKE_CXX_STANDARD 17)

include( scripts/thirdparty.cmake )
include( CTest )
enable_testing()

include_directories("${GCodeOptimizer_SOURCE_DIR}")
add_subdirectory( src )

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_VENDOR "Chooszg")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "It's great")
set(CPACK_PACKAGE_VERSION ${GCodeOptimizer_VERSION_MAJOR}.${GCodeOptimizer_VERSION_MINOR}.${GCodeOptimizer_VERSION_PATCH}-${CMAKE_BUILD_TYPE})
set(CPACK_PACKAGE_VERSION_MAJOR ${GCodeOptimizer_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${GCodeOptimizer_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${GCodeOptimizer_VERSION_PATCH})
include(CPack)