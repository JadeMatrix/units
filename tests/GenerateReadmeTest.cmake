FILE( READ "${PROJECT_SOURCE_DIR}/README.md" README_CONTENTS )

# Temporarily sanitize semicolons to make extracting the C++ code easier
STRING( REPLACE ";" "@" README_CONTENTS "${README_CONTENTS}" )

# CMake regexes supporting lookbehind/lookahead would make this section much
# simpler
STRING( REGEX MATCHALL "```cpp[^`]+```" README_SOURCES_RAW "${README_CONTENTS}" )

# Split out C++ code sections
SET( README_SOURCES )
FOREACH( README_SOURCE_RAW ${README_SOURCES_RAW})
    STRING( LENGTH "${README_SOURCE_RAW}" README_SOURCE_RAW_LEN )
    MATH( EXPR README_SOURCE_RAW_STOP "${README_SOURCE_RAW_LEN} - ( 3 + 6 )" )
    STRING(
        SUBSTRING "${README_SOURCE_RAW}"
        6 "${README_SOURCE_RAW_STOP}"
        README_SOURCE
    )
    LIST( APPEND README_SOURCES "${README_SOURCE}" )
ENDFOREACH()

# Un-sanitize semicolons and concatenate code segments
SET( JOINED_SOURCES )
FOREACH( README_SOURCE ${README_SOURCES} )
    STRING( REPLACE "@" ";" README_SOURCE "${README_SOURCE}" )
    STRING( APPEND JOINED_SOURCES "${README_SOURCE}" )
ENDFOREACH()

# Write concatenated code segments to test source template file
CONFIGURE_FILE(
    "unit_tests/readme_tests.cpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/readme_tests.cpp"
    @ONLY
)
