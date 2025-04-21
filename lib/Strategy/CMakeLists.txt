add_library(Strategy
        OrderedStrategy.cpp
        CustomStrategy/CustomStrategy.cpp
        CustomStrategy/Zone/ZoneManager.cpp
        CustomStrategy/Pattern/PatternManager.cpp
        CustomStrategy/Hunting/HuntingManager.cpp
        CustomStrategy/Statistics/Statistics.cpp
)

target_include_directories(Strategy PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/CustomStrategy
        ${CMAKE_CURRENT_SOURCE_DIR}/CustomStrategy/Zone
        ${CMAKE_CURRENT_SOURCE_DIR}/CustomStrategy/Pattern
        ${CMAKE_CURRENT_SOURCE_DIR}/CustomStrategy/Hunting
        ${CMAKE_CURRENT_SOURCE_DIR}/CustomStrategy/Statistics
)

target_link_libraries(Strategy PUBLIC Field)
