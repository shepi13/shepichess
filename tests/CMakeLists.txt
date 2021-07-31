add_executable(engineTests)

set(SourceFiles testbitboard.cpp)
add_dependencies(engineTests engine)
target_link_libraries(engineTests PRIVATE engine Catch2::Catch2WithMain)
add_dependencies(engineTests Catch2)
target_sources(
    engineTests
    PRIVATE
        ${SourceFiles}
)
include(Catch)
catch_discover_tests(engineTests)