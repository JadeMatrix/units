CMAKE_MINIMUM_REQUIRED( VERSION 3.11 )

PROJECT(
    JadeMatrix-units
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
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>"
)


INSTALL(
    TARGETS units
    EXPORT "JadeMatrix-units-config"
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
    "${PROJECT_BINARY_DIR}/JadeMatrix-units-config-version.cmake"
    COMPATIBILITY SameMinorVersion
)
INSTALL(
    FILES "${PROJECT_BINARY_DIR}/JadeMatrix-units-config-version.cmake"
    DESTINATION "lib/cmake/JadeMatrix-units/"
)

INSTALL(
    EXPORT "JadeMatrix-units-config"
    NAMESPACE "JadeMatrix::units::"
    DESTINATION "lib/cmake/JadeMatrix-units/"
)

EXPORT( EXPORT "JadeMatrix-units-config" FILE "JadeMatrix-units-config.cmake" )
