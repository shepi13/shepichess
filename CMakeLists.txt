cmake_minimum_required(VERSION 3.13.0)

if(NOT DEFINED PROJECT_NAME)
    set(NOT_SUBPROJECT ON)
else()
    set(NOT_SUBPROJECT OFF)
endif()

project(ShepiChess)

if(NOT_SUBPROJECT)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_EXTENSIONS OFF)
    include(CTest)
endif()

include_directories("${PROJECT_SOURCE_DIR}/src")

# Dependencies
add_subdirectory(third_party/spdlog)

# Build
add_subdirectory(src)
add_executable(shepichess "src/main.cpp")
add_dependencies(shepichess engine)
target_link_libraries(shepichess PRIVATE engine)

# Test
if(BUILD_TESTING AND NOT_SUBPROJECT)
    # Testing dependencies
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
    set (BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory(third_party/benchmark)
    add_subdirectory(third_party/Catch2)
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third_party/Catch2/extras)
    # Tests
    add_subdirectory(tests)
    add_subdirectory(benchmark)
endif()
