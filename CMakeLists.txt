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

# Build
add_subdirectory(src)
add_executable(shepichess)
target_link_libraries(shepichess PRIVATE engine)

# Test
if(BUILD_TESTING AND NOT_SUBPROJECT)
    find_package(Catch2 REQUIRED)
    add_subdirectory(tests)
endif()