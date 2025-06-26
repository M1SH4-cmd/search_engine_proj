add_executable(tests
        test_main.cpp
        ConverterJSON_test.cpp
        ${CMAKE_SOURCE_DIR}/src/ConverterJSON.cpp
        ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp
)

target_include_directories(tests PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE
        nlohmann_json::nlohmann_json
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)