cmake_minimum_required(VERSION 3.1)

project(StaticSearcher VERSION 0.1
                       DESCRIPTION "WebAssembly Search For Static Sites"
                       LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)
SET(CMAKE_BUILD_TYPE_INIT "Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER "emcc")
set(CMAKE_CXX_COMPILER "em++")

find_package(cereal CONFIG REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(static_search.js main.cc)

target_link_libraries(static_search.js PRIVATE cereal  ${Boost_LIBRARIES})

set_target_properties(static_search.js PROPERTIES LINK_FLAGS " --bind -O3 --preload-file ../post_data.bin@post_data.bin")
