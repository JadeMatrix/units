CMAKE_MINIMUM_REQUIRED( VERSION 3.11 )

PROJECT(
    units
    VERSION 0.2
    LANGUAGES CXX
)
SET( CMAKE_CXX_STANDARD 11 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )
SET( CMAKE_CXX_EXTENSIONS OFF )


ADD_LIBRARY( units INTERFACE )
TARGET_INCLUDE_DIRECTORIES(
    units
    INTERFACE
        "$<INSTALL_INTERFACE:include/>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>"
)


INSTALL(
    TARGETS units
    EXPORT "jmunits-config"
    RUNTIME DESTINATION "bin/"
    LIBRARY DESTINATION "lib/"
    ARCHIVE DESTINATION "lib/static/"
)
INSTALL(
    DIRECTORY "include/"
    DESTINATION "include/"
)

INCLUDE( CMakePackageConfigHelpers )
WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${CMAKE_CURRENT_BINARY_DIR}/jmunits-config-version.cmake"
    COMPATIBILITY SameMinorVersion
)
INSTALL(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/jmunits-config-version.cmake"
    DESTINATION "lib/cmake/jmunits/"
)

INSTALL(
    EXPORT "jmunits-config"
    NAMESPACE "JadeMatrix::units::"
    DESTINATION "lib/cmake/jmunits/"
)

EXPORT( EXPORT "jmunits-config" FILE "jmunits-config.cmake" )
