
set( SOURCES
    Command.h
    Command.cpp
    Part.h
    Part.cpp
	Optimize.h
	Optimize.cpp
    Position.h
    Position.cpp
)

add_executable( GCodeOptimizer
    ${SOURCES}
    main.cpp
)

add_executable( GCodeTest
    ${SOURCES}
    main.test.cpp
)
target_link_libraries( GCodeTest
    ${GTEST_LIBS}
)

# GTest throws tons of C4996 LNK4099 - Disable it
target_compile_options( GCodeTest PRIVATE "/wd4996" )
#target_link_options( GCodeTest PRIVATE "/ignore:4099" )
add_test( NAME GCodeTest COMMAND ${CMAKE_BINARY_DIR}/GCodeTest )