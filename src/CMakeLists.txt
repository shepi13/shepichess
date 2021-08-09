add_library(engine)

set(SourceFiles bitboard.cpp uci_application.cpp uci_config.cpp)
set(HeaderFiles bitboard.h uci_application.h logging.h position.h)

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
if(WIN32)
target_link_libraries(engine PRIVATE ${PROJECT_SOURCE_DIR}/bin/spdlog/lib/${CMAKE_STATIC_LIBRARY_PREFIX}spdlogd${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
target_link_libraries(engine PRIVATE ${PROJECT_SOURCE_DIR}/bin/spdlog/lib/${CMAKE_STATIC_LIBRARY_PREFIX}spdlog${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

# source group
source_group(
    TREE "${PROJECT_SOURCE_DIR}"
    PREFIX "Headers"
    FILES ${HeaderFiles}
)
