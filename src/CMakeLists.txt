add_library(engine)

set(SourceFiles bitboard.cpp)
set(HeaderFiles bitboard.h)

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
target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/bin/spdlog/include)
target_link_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/bin/spdlog/lib)

# source group
source_group(
    TREE "${PROJECT_SOURCE_DIR}"
    PREFIX "Headers"
    FILES ${HeaderFiles}
)