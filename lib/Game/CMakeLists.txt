add_library(Game IGame.h SlaveGame.h SlaveGame.cpp MasterGame.h MasterGame.cpp)

target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Game PUBLIC Field Strategy)