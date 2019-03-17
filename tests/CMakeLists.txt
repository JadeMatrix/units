ADD_SUBDIRECTORY( "test_utils/" )


# Add command to generate the source for the readme code test
ADD_CUSTOM_COMMAND(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/readme_tests.cpp"
    COMMAND "${CMAKE_COMMAND}"
    ARGS
        -D "PROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
        -D "SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\""
        -D "BINARY_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\""
        -P "${CMAKE_CURRENT_SOURCE_DIR}/GenerateReadmeTest.cmake"
    MAIN_DEPENDENCY "GenerateReadmeTest.cmake"
    DEPENDS
        "${PROJECT_SOURCE_DIR}/README.md"
        "unit_tests/readme_tests.cpp.in"
)


SET( TEST_NAMES
    "Readme_Tests"
    "Linear_Metric"
    "Linear_Imperial"
    "Angular"
)


FOREACH( TEST_NAME ${TEST_NAMES} )
    STRING( REPLACE "_" "" DISPLAY_NAME "${TEST_NAME}" )
    STRING( TOLOWER "${TEST_NAME}" EXECUTABLE_NAME )
    
    ADD_EXECUTABLE( "${EXECUTABLE_NAME}" )
    
    ADD_TEST(
        NAME "${DISPLAY_NAME}"
        COMMAND "${EXECUTABLE_NAME}"
    )
    
    # Command IF( EXISTS ... ) "... is well-defined only for full paths"
    IF( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit_tests/${EXECUTABLE_NAME}.cpp" )
        TARGET_SOURCES(
            "${EXECUTABLE_NAME}"
            PRIVATE "unit_tests/${EXECUTABLE_NAME}.cpp"
        )
    ELSE()
        # Add target built from generated source file
        TARGET_SOURCES(
            "${EXECUTABLE_NAME}"
            PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}.cpp"
        )
    ENDIF()
    
    TARGET_LINK_LIBRARIES( "${EXECUTABLE_NAME}" PRIVATE test_utils )
ENDFOREACH()


# Apply compile options to `test_utils` library, too
FOREACH( NAME "test_utils" ${TEST_NAMES} )
    STRING( TOLOWER "${NAME}" TARGET_NAME )
    IF(
           "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"
    )
        TARGET_COMPILE_OPTIONS(
            "${TARGET_NAME}"
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
                "${TARGET_NAME}"
                PRIVATE
                    "-Wduplicated-cond"
                    "-Wduplicated-branches"
                    "-Wlogical-op"
                    "-Wuseless-cast"
            )
        ENDIF()
    
    ELSEIF( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
        TARGET_COMPILE_OPTIONS(
            "${TARGET_NAME}"
            PRIVATE
                "/DEBUG"
                "/WX"
                "/Wall"
        )
    ENDIF()
ENDFOREACH()
