add_executable(engineTests)

set(SourceFiles test_position.cpp test_bitboard.cpp test_uci_application.cpp test_hash_table.cpp)

target_sources(
    engineTests
    PRIVATE
        ${SourceFiles}
)

add_dependencies(engineTests engine)
target_link_libraries(engineTests PRIVATE engine)

add_dependencies(engineTests Catch2)
target_include_directories(engineTests PUBLIC ${PROJECT_SOURCE_DIR}/third_party/Catch2/src)
target_link_libraries(engineTests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(engineTests)
