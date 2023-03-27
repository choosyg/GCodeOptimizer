
set( SOURCES
    Command.h
    Command.cpp
    GCodeFile.h
    GCodeFile.cpp
    Part.h
    Part.cpp
	Optimize.h
	Optimize.cpp
    Position.h
    Position.cpp
)

add_library( GCode
    ${SOURCES}
)

add_executable( GCodeTest
    Command.test.cpp
    Optimize.test.cpp
    Part.test.cpp
    Position.test.cpp
)
target_link_libraries( GCodeTest gtest_main GCode )
if (MSVC)
    # GTest throws tons of C4996 LNK4099 - Disable it
    target_compile_options( GCodeTest PRIVATE "/wd4996" )
endif (MSVC)

#target_link_options( GCodeTest PRIVATE "/ignore:4099" )
add_test( NAME GCodeTest COMMAND ${CMAKE_BINARY_DIR}/GCodeTest )