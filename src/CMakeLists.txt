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
target_link_libraries(engine PRIVATE spdlog)
source_group(
    TREE "${PROJECT_SOURCE_DIR}"
    PREFIX "Headers"
    FILES ${HeaderFiles}
)