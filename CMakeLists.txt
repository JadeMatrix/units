CMAKE_MINIMUM_REQUIRED( VERSION 3.11 #[[
    Requiring 3.12+ (fallback exists):
        ADD_LIBRARY( foo OBJECT ) TARGET_LINK_LIBRARIES( foo ... )
    Requiring 3.11+:
        WRITE_BASIC_PACKAGE_VERSION_FILE( ... SameMinorVersion ... )
        ADD_*( foo ) ... TARGET_SOURCES( foo ... )
    Requiring 3.10+:
        INCLUDE( doctest )
]] )

PROJECT(
    "JadeMatrix-units"
    VERSION 0.3
    LANGUAGES CXX
)

SET( CMAKE_CXX_STANDARD 11 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )
SET( CMAKE_CXX_EXTENSIONS OFF )

LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/" )
IF( EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" )
    INCLUDE( "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" )
    CONAN_SET_FIND_PATHS()
ENDIF()

INCLUDE( CTest )
INCLUDE( GNUInstallDirs )


ADD_LIBRARY( units INTERFACE )
TARGET_INCLUDE_DIRECTORIES(
    units
    INTERFACE
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)


INSTALL(
    TARGETS units
    EXPORT "jadematrix-units-config"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT core
)
INSTALL(
    DIRECTORY "include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT core
)

INCLUDE( CMakePackageConfigHelpers )
WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${PROJECT_BINARY_DIR}/jadematrix-units-config-version.cmake"
    COMPATIBILITY SameMinorVersion
)
INSTALL(
    FILES "${PROJECT_BINARY_DIR}/jadematrix-units-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/JadeMatrix-units/"
    COMPONENT cmake
)

INSTALL(
    EXPORT "jadematrix-units-config"
    NAMESPACE "JadeMatrix::units::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/JadeMatrix-units/"
    COMPONENT cmake
)

EXPORT( EXPORT "jadematrix-units-config" FILE "jadematrix-units-config.cmake" )


IF( BUILD_TESTING )
    ADD_SUBDIRECTORY( "tests/" )
ENDIF()
