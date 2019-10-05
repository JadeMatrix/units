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
        -D "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}"
        -D "SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
        -D "BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}"
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
    "mult_div_promotion"
    "ratio_conversion"
    "per_conversion"
)
    UNITS_ADD_COMPILE_TEST( "${COMPILE}"
        FILE "pass/${COMPILE}.cpp"
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
        FILE "fail/${COMPILE}.cpp"
        WILL_FAIL
        COMPILE_OPTIONS
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
ENDFOREACH()


# Test for warn on narrowing conversions
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




FUNCTION( DEFINE_CONSTRUCT_PASSTHROUGH_TEST_CASE
    RHS_TYPE
    ADD_STR
    LHS
    RHS
)
    SET( OPERAND_plu "+" )
    SET( OPERAND_min "-" )
    SET( OPERAND_mul "*" )
    SET( OPERAND_div "/" )
    SET( OPERAND_mod "%" )
    
    SET( TEST_NAME
        "custom_value_type_operator_construct_from_unit${ADD_STR}"
    )
    SET( SOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.cpp" )
    
    FILE( REMOVE "${SOURCE_FILE}" )
    FOREACH( LINE IN ITEMS
        "#include \"${PROJECT_SOURCE_DIR}/tests/suites/custom_value_type.hpp\"\n"
        "#include <type_traits>  // is_same\n"
        "#include <units/angular.hpp>\n"
        "#include <units/linear.hpp>\n"
        "#include <units/temporal.hpp>\n"
        "namespace units = ::JadeMatrix::units;\n"
        "units::${RHS}< ${RHS_TYPE} > val1{ 8304.51L };\n"
        "units::${LHS}< ::custom::bar > val2{ val1 };\n"
    )
        FILE( APPEND "${SOURCE_FILE}" "${LINE}" )
    ENDFOREACH()
    UNITS_ADD_COMPILE_TEST( "${TEST_NAME}"
        FILE "${SOURCE_FILE}"
        COMPILE_OPTIONS
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
ENDFUNCTION()




FUNCTION( DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE
    RHS_TYPE
    ADD_STR
    OPERAND
    LHS
    RHS
)
    SET( OPERAND_plu "+" )
    SET( OPERAND_min "-" )
    SET( OPERAND_mul "*" )
    SET( OPERAND_div "/" )
    SET( OPERAND_mod "%" )
    
    SET( TEST_NAME
        "custom_value_type_operator_${OPERAND}_passthrough${ADD_STR}"
    )
    SET( SOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.cpp" )
    
    FILE( REMOVE "${SOURCE_FILE}" )
    FOREACH( LINE IN ITEMS
        "#include \"${PROJECT_SOURCE_DIR}/tests/suites/custom_value_type.hpp\"\n"
        "#include <type_traits>  // is_same\n"
        "#include <units/angular.hpp>\n"
        "#include <units/linear.hpp>\n"
        "#include <units/temporal.hpp>\n"
        "namespace units = ::JadeMatrix::units;\n"
        "units::${LHS}< ::custom::bar > val1{ 123.4L };\n"
        "units::${RHS}< ${RHS_TYPE} > val2{ 123.4L };\n"
        "static_assert( std::is_same<\n"
        "    decltype( val1 ${OPERAND_${OPERAND}} val2 )::value_type,\n"
        "    ::custom::tagged< ::custom::bar, ::custom::bar::${OPERAND}_tag >\n"
        ">::value, \"unit operator ${OPERAND_${OPERAND}} passthrough failed\" );\n"
    )
        FILE( APPEND "${SOURCE_FILE}" "${LINE}" )
    ENDFOREACH()
    UNITS_ADD_COMPILE_TEST( "${TEST_NAME}"
        FILE "${SOURCE_FILE}"
        COMPILE_OPTIONS
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
ENDFUNCTION()




FUNCTION( DEFINE_COMPARE_PASSTHROUGH_TEST_CASE
    RHS_TYPE
    ADD_STR
    OPERAND
    LHS
    RHS
)
    SET( OPERAND_eq "==" )
    SET( OPERAND_ne "!=" )
    SET( OPERAND_gt ">"  )
    SET( OPERAND_lt "<"  )
    SET( OPERAND_ge ">=" )
    SET( OPERAND_le "<=" )
    
    SET( TEST_NAME
        "custom_value_type_operator_${OPERAND}_passthrough${ADD_STR}"
    )
    SET( SOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.cpp" )
    
    FILE( REMOVE "${SOURCE_FILE}" )
    FOREACH( LINE IN ITEMS
        "#include \"${PROJECT_SOURCE_DIR}/tests/suites/custom_value_type.hpp\"\n"
        "#include <type_traits>  // is_same\n"
        "#include <units/angular.hpp>\n"
        "#include <units/linear.hpp>\n"
        "#include <units/temporal.hpp>\n"
        "namespace units = ::JadeMatrix::units;\n"
        "units::${LHS}< ::custom::bar > val1{ 123.4L };\n"
        "units::${RHS}< ${RHS_TYPE} > val2{ 123.4L };\n"
        "static_assert( std::is_same<\n"
        "    decltype( val1 ${OPERAND_${OPERAND}} val2 ),\n"
        "    ::custom::tagged< ::custom::bar::cmp, ::custom::bar::${OPERAND}_tag >\n"
        ">::value, \"unit operator ${OPERAND_${OPERAND}} passthrough failed\" );\n"
    )
        FILE( APPEND "${SOURCE_FILE}" "${LINE}" )
    ENDFOREACH()
    UNITS_ADD_COMPILE_TEST( "${TEST_NAME}"
        FILE "${SOURCE_FILE}"
        COMPILE_OPTIONS
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
ENDFUNCTION()




FUNCTION( DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE
    RHS_TYPE
    ADD_STR
    OPERAND
    LHS
    RHS
)
    SET( OPERAND_plu "+" )
    SET( OPERAND_min "-" )
    SET( OPERAND_mul "*" )
    SET( OPERAND_div "/" )
    SET( OPERAND_mod "%" )
    
    SET( TEST_NAME
        "custom_value_type_operator_${OPERAND}_assign_no_passthrough${ADD_STR}"
    )
    SET( SOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.cpp" )
    
    FILE( REMOVE "${SOURCE_FILE}" )
    FOREACH( LINE IN ITEMS
        "#include \"${PROJECT_SOURCE_DIR}/tests/suites/custom_value_type.hpp\"\n"
        "#include <type_traits>  // is_same\n"
        "#include <units/angular.hpp>\n"
        "#include <units/linear.hpp>\n"
        "#include <units/temporal.hpp>\n"
        "namespace units = ::JadeMatrix::units;\n"
        "units::${LHS}< ::custom::bar > val1{ 123.4L };\n"
        "units::${RHS}< ${RHS_TYPE} > val2{ 123.4L };\n"
        "static_assert( std::is_same<\n"
        "    decltype( val1 ${OPERAND_${OPERAND}}= val2 ),\n"
        "    units::${LHS}< ::custom::bar >&\n"
        ">::value, \"unit operator ${OPERAND_${OPERAND}}= no passthrough failed\" );\n"
    )
        FILE( APPEND "${SOURCE_FILE}" "${LINE}" )
    ENDFOREACH()
    UNITS_ADD_COMPILE_TEST( "${TEST_NAME}"
        FILE "${SOURCE_FILE}"
        COMPILE_OPTIONS
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
ENDFUNCTION()

FOREACH( VARIANT IN ITEMS
    "::custom::bar\;"
    "long double\;_with_convertible_type"
)
    SET( VARIANT ${VARIANT} ) # LIST-ify
    LIST( GET VARIANT 0 RHS_TYPE )
    LIST( GET VARIANT 1 ADD_STR  )
    
    DEFINE_CONSTRUCT_PASSTHROUGH_TEST_CASE ( "${RHS_TYPE}" "${ADD_STR}"           "terarevolutions"  "petadegrees" )
    
    DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( "${RHS_TYPE}" "${ADD_STR}" "mul"         "revolutions"      "radians" )
    DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( "${RHS_TYPE}" "${ADD_STR}" "div"         "nanoseconds"        "hours" )
    DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( "${RHS_TYPE}" "${ADD_STR}" "mod"         "dozen_hours" "microminutes" )
    DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( "${RHS_TYPE}" "${ADD_STR}" "plu"              "meters"    "kiloyards" )
    DEFINE_ARITHMETIC_PASSTHROUGH_TEST_CASE( "${RHS_TYPE}" "${ADD_STR}" "min"          "arcseconds"   "arcminutes" )
    
    DEFINE_COMPARE_PASSTHROUGH_TEST_CASE   ( "${RHS_TYPE}" "${ADD_STR}" "eq"           "gigameters"   "nanometers" )
    DEFINE_COMPARE_PASSTHROUGH_TEST_CASE   ( "${RHS_TYPE}" "${ADD_STR}" "ne"          "revolutions"  "revolutions" )
    DEFINE_COMPARE_PASSTHROUGH_TEST_CASE   ( "${RHS_TYPE}" "${ADD_STR}" "gt"              "minutes"      "seconds" )
    DEFINE_COMPARE_PASSTHROUGH_TEST_CASE   ( "${RHS_TYPE}" "${ADD_STR}" "lt"                 "feet"       "inches" )
    DEFINE_COMPARE_PASSTHROUGH_TEST_CASE   ( "${RHS_TYPE}" "${ADD_STR}" "ge"       "nanoarcseconds" "centiradians" )
    DEFINE_COMPARE_PASSTHROUGH_TEST_CASE   ( "${RHS_TYPE}" "${ADD_STR}" "le" "dozen_nautical_miles"  "centimeters" )
    
    DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE ( "${RHS_TYPE}" "${ADD_STR}" "plu"         "revolutions"      "radians" )
    DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE ( "${RHS_TYPE}" "${ADD_STR}" "min"         "nanoseconds"        "hours" )
    DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE ( "${RHS_TYPE}" "${ADD_STR}" "mul"         "dozen_hours" "microminutes" )
    DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE ( "${RHS_TYPE}" "${ADD_STR}" "div"              "meters"    "kiloyards" )
    DEFINE_ASSIGN_NO_PASSTHROUGH_TEST_CASE ( "${RHS_TYPE}" "${ADD_STR}" "mod"          "arcseconds"   "arcminutes" )
ENDFOREACH()
