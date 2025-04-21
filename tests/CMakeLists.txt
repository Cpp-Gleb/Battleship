cmake_minimum_required(VERSION 3.14)

include(FetchContent)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Задаем FetchContent для Google Test
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(runTests
        GameTest.cpp
        CommandHandlerTest.cpp
)

target_link_libraries(runTests
        GTest::gtest_main
        Game
        Strategy
        IO
        Field
        FieldStorage
)

target_include_directories(runTests PUBLIC
        ${PROJECT_SOURCE_DIR}/lib
)

include(GoogleTest)
gtest_discover_tests(runTests)
