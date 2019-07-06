# Add a test that checks that some code compiles.
# Signature:
#   UNITS_ADD_COMPILE_TEST(
#       <base_name>
#       [ WILL_FAIL ]
#       [ FILE <filename> ]
#       [ NAME <name> ]
#       [ COMPILE_OPTIONS <option> ... ]
#       [ DEPENDS_TARGETS <target> ... ]
#   )
# Defaults:
#   FILE    "${BASENAME}.cpp"
#   NAME    "${BASENAME}_compiles"
FUNCTION( UNITS_ADD_COMPILE_TEST BASENAME )
    CMAKE_PARSE_ARGUMENTS( TEST
        "WILL_FAIL"
        "FILE;NAME"
        "COMPILE_OPTIONS;DEPENDS_TARGETS"
        ${ARGN}
    )
    
    IF( NOT TEST_FILE )
        SET( TEST_FILE "${BASENAME}.cpp" )
    ENDIF()
    IF( NOT TEST_NAME )
        SET( TEST_NAME "${BASENAME}" )
    ENDIF()
    
    # Add library because we aren't testing failure to link; OBJECT because it
    # has the least semantics; EXCLUDE_FROM_ALL is important in case WILL_FAIL
    # is set on this test
    ADD_LIBRARY( ${TEST_NAME}_compiles
        OBJECT
        EXCLUDE_FROM_ALL
        "${TEST_FILE}"
    )
    TARGET_LINK_LIBRARIES( ${TEST_NAME}_compiles PRIVATE units )
    TARGET_COMPILE_OPTIONS( ${TEST_NAME}_compiles
        PRIVATE ${TEST_COMPILE_OPTIONS}
    )
    
    SET( DEPENDS_TESTS )
    FOREACH( DEPENDS_TARGET IN LISTS DEPENDS_TARGETS )
        SET( DEPENDS_TEST_NAME "${TEST_NAME}_compiles_${DEPENDS_TARGET}" )
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
            --target "${TEST_NAME}_compiles"
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
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/readme_tests.cpp"
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
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/readme_tests.cpp"
)
UNITS_ADD_COMPILE_TEST( readme
    FILE "${CMAKE_CURRENT_BINARY_DIR}/readme_tests.cpp"
    COMPILE_OPTIONS
        ${UNITS_COMPILE_OPTIONS}
    DEPENDS_TARGETS readme_source
)


# Positive compile tests
FOREACH( COMPILE IN ITEMS
    "ratio_conversion"
    "per_conversion"
)
    UNITS_ADD_COMPILE_TEST( "${COMPILE}"
        COMPILE_OPTIONS
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
ENDFOREACH()


# Negative compile tests
FOREACH( COMPILE IN ITEMS
    "incompatible_unit_conversion"
)
    UNITS_ADD_COMPILE_TEST( "${COMPILE}"
        WILL_FAIL
        COMPILE_OPTIONS
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
ENDFOREACH()


# Compiler-specific tests (unfortunately)
IF(
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
    OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"
)
    UNITS_ADD_COMPILE_TEST( "implicit_storage_conversion"
        COMPILE_OPTIONS ${UNITS_COMPILE_OPTIONS}
    )
    UNITS_ADD_COMPILE_TEST( "implicit_storage_conversion"
        WILL_FAIL
        NAME "implicit_storage_conversion_compiles_warn"
        COMPILE_OPTIONS
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
ENDIF()
