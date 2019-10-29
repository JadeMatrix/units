FIND_PACKAGE( doctest 2.3.3 REQUIRED COMPONENTS doctest )
INCLUDE( doctest )


# Regular runnable unit tests
ADD_CUSTOM_TARGET( tests ALL )

FOREACH( SUITE IN ITEMS
    "angular"
    "core_type_detection"
    "custom_unit"
    "custom_value_type"
    "digital"
    "linear_imperial"
    "linear_metric"
    "linear_nautical"
    "ratio"
    "temporal"
)
    ADD_EXECUTABLE( ${SUITE}_tests )
    TARGET_SOURCES( ${SUITE}_tests PRIVATE "${SUITE}.cpp" )
    TARGET_LINK_LIBRARIES( ${SUITE}_tests PRIVATE units doctest::doctest )
    TARGET_COMPILE_OPTIONS( ${SUITE}_tests
        PRIVATE
            ${UNITS_COMPILE_OPTIONS}
            ${UNITS_COMPILE_OPTIONS_CONVERSION}
    )
    
    ADD_DEPENDENCIES( tests ${SUITE}_tests )
    DOCTEST_DISCOVER_TESTS( ${SUITE}_tests )
ENDFOREACH()
