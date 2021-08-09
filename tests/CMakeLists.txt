add_executable(engineTests)

set(SourceFiles testbitboard.cpp test_uci_application.cpp)

target_sources(
    engineTests
    PRIVATE
        ${SourceFiles}
)

add_dependencies(engineTests engine)
target_link_libraries(engineTests PRIVATE engine)

add_dependencies(engineTests Catch2)
target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/bin/Catch2/include)
target_link_libraries(engine PRIVATE ${PROJECT_SOURCE_DIR}/bin/Catch2/lib/Catch2Main${CMAKE_STATIC_LIBRARY_SUFFIX})
target_link_libraries(engine PRIVATE ${PROJECT_SOURCE_DIR}/bin/Catch2/lib/Catch2${CMAKE_STATIC_LIBRARY_SUFFIX})

include(Catch)
catch_discover_tests(engineTests)
