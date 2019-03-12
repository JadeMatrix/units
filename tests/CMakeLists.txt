ADD_SUBDIRECTORY( "test_utils/" )


FOREACH( TEST_NAME
    "Unit_Tests"
)
    STRING( REPLACE "_" "" DISPLAY_NAME "${TEST_NAME}" )
    STRING( TOLOWER "${TEST_NAME}" EXECUTABLE_NAME )

    ADD_EXECUTABLE( "${EXECUTABLE_NAME}" )

    ADD_TEST(
        NAME "UnitTests"
        COMMAND "${EXECUTABLE_NAME}"
    )

    TARGET_SOURCES(
        "${EXECUTABLE_NAME}"
        PRIVATE "unit_tests/${EXECUTABLE_NAME}.cpp"
    )
    TARGET_LINK_LIBRARIES( "${EXECUTABLE_NAME}" PRIVATE test_utils )

    IF(
           "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"
    )
        TARGET_COMPILE_OPTIONS(
            "${EXECUTABLE_NAME}"
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
                "${EXECUTABLE_NAME}"
                PRIVATE
                    "-Wduplicated-cond"
                    "-Wduplicated-branches"
                    "-Wlogical-op"
                    "-Wuseless-cast"
            )
        ENDIF()
    
    ELSEIF( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
        TARGET_COMPILE_OPTIONS(
            "${EXECUTABLE_NAME}"
            PRIVATE
                "/DEBUG"
                "/WX"
                "/Wall"
        )
    ENDIF()
    
ENDFOREACH()
