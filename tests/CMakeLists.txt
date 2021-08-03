add_executable(engineTests)

set(SourceFiles testbitboard.cpp)

target_sources(
    engineTests
    PRIVATE
        ${SourceFiles}
)

add_dependencies(engineTests engine)
target_link_libraries(engineTests PRIVATE engine)

add_dependencies(engineTests Catch2)
target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/bin/Catch2/include)
target_link_directories(engine PRIVATE ${PROJECT_SOURCE_DIR}/bin/Catch2/lib)
target_link_libraries(engine PRIVATE libCatch2Main.a)
target_link_libraries(engine PRIVATE libCatch2.a)

include(Catch)
catch_discover_tests(engineTests)
