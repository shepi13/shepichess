add_library(engine)

set(
    SourceFiles 
    bitboard.cpp 
    hash_table.cpp 
    position.cpp
    uci_application.cpp 
    uci_config.cpp 
)
set(
    HeaderFiles 
    bitboard.h 
    eval.h
    hash_table.h 
    position.h 
    uci_application.h 
    uci_config.h 
)

target_sources( 
    engine
    PRIVATE
        ${SourceFiles}
    PUBLIC
        ${HeaderFiles}
)
target_compile_features(engine PRIVATE cxx_std_17)

# spdlog
add_dependencies(engine spdlog)
target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/third_party/spdlog/include)
target_link_libraries(engine PRIVATE spdlog::spdlog)
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(engine PRIVATE tbb)
endif()

# source group
source_group(
    TREE "${PROJECT_SOURCE_DIR}"
    PREFIX "Headers"
    FILES ${HeaderFiles}
)
