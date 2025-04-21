add_library(FieldStorage IFieldStorage.h FieldCompressor.h FieldCompressor.cpp CompressedFieldStorage.h CompressedFieldStorage.cpp)

target_include_directories(FieldStorage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
