add_library(IO CommandHandler.h CommandHandler.cpp)

target_include_directories(IO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(IO PUBLIC Game)