FIND_PACKAGE( doctest 2.3.3 REQUIRED COMPONENTS doctest )
INCLUDE( doctest )


# ADD_SUBDIRECTORY( "testing/" )


# Add command to generate the source for the readme code test
ADD_CUSTOM_COMMAND(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/suites/readme_tests.cpp"
    COMMAND "${CMAKE_COMMAND}"
    ARGS
        -D "PROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
        -D "SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\""
        -D "BINARY_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\""
        -P "${CMAKE_CURRENT_SOURCE_DIR}/GenerateReadmeTest.cmake"
    MAIN_DEPENDENCY "GenerateReadmeTest.cmake"
    DEPENDS
        "${PROJECT_SOURCE_DIR}/README.md"
        "suites/readme_tests.cpp.in"
)


ADD_CUSTOM_TARGET( tests ALL )

FOREACH( SUITE
    "angular"
    "custom_unit"
    "linear_imperial"
    "linear_metric"
    "linear_nautical"
    "readme_tests"
    "temporal"
)
    ADD_EXECUTABLE( ${SUITE}_tests )
    # TARGET_SOURCES( ${SUITE}_tests PRIVATE "suites/${SUITE}.cpp" )
    
    # Command IF( EXISTS ... ) "... is well-defined only for full paths"
    IF( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/suites/${SUITE}.cpp" )
        TARGET_SOURCES(
            ${SUITE}_tests
            PRIVATE "suites/${SUITE}.cpp"
        )
    ELSE()
        # Add target built from generated source file
        TARGET_SOURCES(
            ${SUITE}_tests
            PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/suites/${SUITE}.cpp"
        )
    ENDIF()
    
    TARGET_LINK_LIBRARIES( ${SUITE}_tests PRIVATE units doctest::doctest )
    
    IF(
           "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
        OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"
    )
        TARGET_COMPILE_OPTIONS(
            ${SUITE}_tests
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
                ${SUITE}_tests
                PRIVATE
                    "-Wduplicated-cond"
                    "-Wduplicated-branches"
                    "-Wlogical-op"
                    "-Wuseless-cast"
            )
        ENDIF()
    ELSEIF( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
        TARGET_COMPILE_OPTIONS(
            ${SUITE}_tests
            PRIVATE
                "/DEBUG"
                "/WX"
                "/Wall"
        )
    ENDIF()
    
    ADD_DEPENDENCIES( tests ${SUITE}_tests )
    DOCTEST_DISCOVER_TESTS( ${SUITE}_tests )
ENDFOREACH()
