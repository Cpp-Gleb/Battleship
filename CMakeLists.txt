cmake_minimum_required(VERSION 3.12)

project(
        labwork5
        VERSION 1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(tests)

enable_testing()
