ADD_EXECUTABLE( unit_tests )

ADD_TEST(
    NAME "UnitTests"
    COMMAND unit_tests
)

TARGET_SOURCES(
    unit_tests
    PRIVATE "unit_tests/src/unit_tests.cpp"
)
TARGET_LINK_LIBRARIES( unit_tests PRIVATE units )

IF(
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
    OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
    OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
    OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"
)
    TARGET_COMPILE_OPTIONS(
        unit_tests
        PRIVATE
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
            "-Wconversion"
            "-Wsign-conversion"
            "-Wnull-dereference"
            "-Wdouble-promotion"
            "-Wformat=2"
    )
    IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        TARGET_COMPILE_OPTIONS(
            unit_tests
            PRIVATE
                "-Wduplicated-cond"
                "-Wduplicated-branches"
                "-Wlogical-op"
                "-Wuseless-cast"
        )
    ENDIF()
ELSEIF( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    TARGET_COMPILE_OPTIONS(
        unit_tests
        PRIVATE
            "/DEBUG"
            "/WX"
            "/Wall"
    )
ENDIF()
