cmake_minimum_required(VERSION 3.20)
project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
set(JSON_BuildTests OFF CACHE INTERNAL "")

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(nlohmann_json)

add_executable(search_engine
        src/main.cpp
        src/ConverterJSON.cpp
        include/ConverterJSON.h
)

target_include_directories(search_engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(search_engine PRIVATE
        nlohmann_json::nlohmann_json
)

add_executable(tests
        test/test_main.cpp
        test/ConverterJSON_test.cpp
        src/ConverterJSON.cpp
)

target_include_directories(tests PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE
        nlohmann_json::nlohmann_json
        gtest_main
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)