project(tests)
set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
set(JSON_BuildTests OFF CACHE INTERNAL "")

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

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
        gtest
)

include(GoogleTest)
gtest_discover_tests(tests)