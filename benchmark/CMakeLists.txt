add_executable(engineBenchmarks)
set(SourceFiles benchmarks.cpp)

target_sources(
    engineBenchmarks
    PRIVATE
        ${SourceFiles}
)

add_dependencies(engineBenchmarks engine)
target_link_libraries(engineBenchmarks PRIVATE engine)

add_dependencies(engineBenchmarks benchmark)
target_link_libraries(engineBenchmarks PRIVATE benchmark::benchmark benchmark::benchmark_main)