add_library(Field Field.h Field.cpp Ship.h Ship.cpp
)

target_include_directories(Field PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Field PUBLIC FieldStorage)