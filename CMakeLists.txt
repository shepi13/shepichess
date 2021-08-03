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
include(ExternalProject)
ExternalProject_Add(
    spdlog
    PREFIX spdlog
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/third_party/spdlog
    CMAKE_ARGS 
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DSPDLOG_BUILD_SHARED=OFF
    -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/bin/spdlog
)
link_directories(${PROJECT_SOURCE_DIR}/bin/spdlog/lib)
include_directories(${PROJECT_SOURCE_DIR}/bin/spdlog/include)

# Build
add_subdirectory(src)
add_executable(shepichess "src/main.cpp")
add_dependencies(shepichess engine)
target_link_libraries(shepichess PRIVATE engine)

# Test
if(BUILD_TESTING AND NOT_SUBPROJECT)
    ExternalProject_Add(
        Catch2
        PREFIX Catch2
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/third_party/Catch2
        CMAKE_ARGS 
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/bin/Catch2
    )
    link_directories(${PROJECT_SOURCE_DIR}/bin/Catch2/lib)
    include_directories(${PROJECT_SOURCE_DIR}/bin/Catch2/include)
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third_party/Catch2/extras)
    add_subdirectory(tests)
endif()
