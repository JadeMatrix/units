IF(
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
    OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
    OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
    OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"
)
    SET( UNITS_COMPILE_OPTIONS
        "-g"
        "-Werror"
        "-Wall"
        "-Wextra"
        "-Wshadow"
        "-Wnon-virtual-dtor"
        "-Wold-style-cast"
        "-Wcast-align"
        "-Wunused"
        "-Woverloaded-virtual"
        "-Wpedantic"
        "-Wnull-dereference"
        "-Wformat=2"
    )
    SET( UNITS_COMPILE_OPTIONS_CONVERSION
        "-Wconversion"
        "-Wsign-conversion"
        "-Wdouble-promotion"
    )
    
    IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        SET( UNITS_COMPILE_OPTIONS
            "-Wduplicated-cond"
            "-Wduplicated-branches"
            "-Wlogical-op"
            "-Wuseless-cast"
        )
    ENDIF()
ELSEIF( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    SET( UNITS_COMPILE_OPTIONS
        "/DEBUG"
        "/WX"
        "/Wall"
    )
    SET( UNITS_COMPILE_OPTIONS_CONVERSION #[[ TODO ]] )
ENDIF()


ADD_SUBDIRECTORY( "suites/" )
ADD_SUBDIRECTORY( "compile/" )
