cmake_minimum_required(VERSION 3.5)
project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(nlohmann_json)

add_executable(search_engine
        src/main.cpp
        src/ConverterJSON.cpp
        include/ConverterJSON.h
        src/InvertedIndex.cpp
        include/InvertedIndex.h
        src/SearchServer.cpp
        include/SearchServer.h
)

set_property(TARGET search_engine PROPERTY ENABLE_EXPORTS ON)

target_include_directories(search_engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(search_engine PRIVATE
        nlohmann_json::nlohmann_json
)



enable_testing()
add_subdirectory(test)