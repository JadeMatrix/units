ADD_LIBRARY( test_utils STATIC )

TARGET_SOURCES( test_utils PRIVATE "src/main.cpp" )
TARGET_INCLUDE_DIRECTORIES( test_utils PUBLIC "include/" )
TARGET_LINK_LIBRARIES( test_utils PUBLIC units )
