FIND_PACKAGE( doctest 2.3.3 REQUIRED COMPONENTS doctest )
INCLUDE( doctest )


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
        "-Wconversion"
        "-Wsign-conversion"
        "-Wnull-dereference"
        "-Wdouble-promotion"
        "-Wformat=2"
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
ENDIF()


# Add a test that checks that some code compiles.
# Signature:
#   UNITS_ADD_COMPILE_TEST(
#       <base_name>
#       [ WILL_FAIL ]
#       [ FILE <filename> ]
#       [ NAME <name> ]
#       [ DEPENDS_TARGETS <target> ... ]
#   )
# Defaults:
#   FILE    "compile/${BASENAME}.cpp"
#   NAME    "${BASENAME}_compile_test"
FUNCTION( UNITS_ADD_COMPILE_TEST BASENAME )
    CMAKE_PARSE_ARGUMENTS( TEST
        "WILL_FAIL"
        "FILE;NAME"
        "DEPENDS_TARGETS"
        ${ARGN}
    )
    
    IF( NOT TEST_FILE )
        SET( TEST_FILE "compile/${BASENAME}.cpp" )
    ENDIF()
    
    # Add library because we aren't testing failure to link; OBJECT because it
    # has the least semantics; EXCLUDE_FROM_ALL is important in case WILL_FAIL
    # is set on this test
    ADD_LIBRARY( ${BASENAME}_compile_test
        OBJECT
        EXCLUDE_FROM_ALL
        "${TEST_FILE}"
    )
    TARGET_LINK_LIBRARIES( ${BASENAME}_compile_test PRIVATE units )
    TARGET_COMPILE_OPTIONS( ${BASENAME}_compile_test
        PRIVATE ${UNITS_COMPILE_OPTIONS}
    )
    
    IF( NOT TEST_NAME )
        SET( TEST_NAME "${BASENAME}_compile_test" )
    ENDIF()
    
    SET( DEPENDS_TESTS )
    FOREACH( DEPENDS_TARGET IN LISTS DEPENDS_TARGETS )
        SET( DEPENDS_TEST_NAME "${BASENAME}_compile_test_${DEPENDS_TARGET}" )
        ADD_TEST(
            NAME ${DEPENDS_TEST_NAME}
            COMMAND "${CMAKE_COMMAND}"
                --build "${CMAKE_BINARY_DIR}"
                --target "${DEPENDS_TARGET}"
            WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
        )
        LIST( APPEND DEPENDS_TESTS ${DEPENDS_TEST_NAME} )
    ENDFOREACH()
    
    # Test to build the OBJECT library
    ADD_TEST(
        NAME "${TEST_NAME}"
        COMMAND "${CMAKE_COMMAND}"
            --build "${CMAKE_BINARY_DIR}"
            --target "${BASENAME}_compile_test"
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    )
    SET_TESTS_PROPERTIES( "${TEST_NAME}"
        PROPERTIES
            FIXTURES_SETUP "${DEPENDS_TESTS}"
            WILL_FAIL "${TEST_WILL_FAIL}"
    )
ENDFUNCTION()


# Add command to generate the source for the readme code test
SET( README_GEN_SCRIPT
    "${PROJECT_SOURCE_DIR}/CMakeModules/unitsGenerateReadmeTest.cmake"
)
ADD_CUSTOM_COMMAND(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/compile/readme_tests.cpp"
    COMMAND "${CMAKE_COMMAND}"
    ARGS
        -D "PROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
        -D "SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\""
        -D "BINARY_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\""
        -P "${README_GEN_SCRIPT}"
    MAIN_DEPENDENCY "${README_GEN_SCRIPT}"
    DEPENDS "${PROJECT_SOURCE_DIR}/README.md"
)
ADD_CUSTOM_TARGET( readme_source
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/compile/readme_tests.cpp"
)
UNITS_ADD_COMPILE_TEST( readme
    FILE "${CMAKE_CURRENT_BINARY_DIR}/compile/readme_tests.cpp"
    DEPENDS_TARGETS readme_source
)


# Positive compile tests
FOREACH( COMPILE IN ITEMS
    # 
)
    UNITS_ADD_COMPILE_TEST( "${COMPILE}" )
ENDFOREACH()


# Negative compile tests
FOREACH( COMPILE IN ITEMS
    "incompatible_unit_conversion"
)
    UNITS_ADD_COMPILE_TEST( "${COMPILE}" WILL_FAIL )
ENDFOREACH()


# Regular runnable unit tests
ADD_CUSTOM_TARGET( tests ALL )
FOREACH( SUITE IN ITEMS
    "angular"
    "custom_unit"
    "linear_imperial"
    "linear_metric"
    "linear_nautical"
    "temporal"
)
    ADD_EXECUTABLE( ${SUITE}_tests )
    
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
    
    TARGET_LINK_LIBRARIES(  ${SUITE}_tests PRIVATE units doctest::doctest   )
    TARGET_COMPILE_OPTIONS( ${SUITE}_tests PRIVATE ${UNITS_COMPILE_OPTIONS} )
    
    ADD_DEPENDENCIES( tests ${SUITE}_tests )
    DOCTEST_DISCOVER_TESTS( ${SUITE}_tests )
ENDFOREACH()
