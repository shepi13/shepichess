add_library(engine)

set(SourceFiles main.cpp)
set(HeaderFiles bitboard.h)

target_sources( 
    engine
    PRIVATE
        ${SourceFiles}
    PUBLIC
        ${HeaderFiles}
)
target_compile_features(engine PRIVATE cxx_std_17)
source_group(
    TREE "${PROJECT_SOURCE_DIR}"
    PREFIX "Headers"
    FILES ${HeaderFiles}
)