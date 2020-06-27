cmake_minimum_required(VERSION 3.1)

project(StaticSearcher VERSION 0.1
                       DESCRIPTION "WebAssembly Search For Static Sites"
                       LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

make_includable(src/web/main.cc src/main.web.cc)

make_includable(src/web/index.html src/index.html)



find_package(nlohmann_json REQUIRED)
find_package(cereal REQUIRED)
find_package(cxxopts REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.50 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})


add_executable(main src/main.cc)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json cereal  ${Boost_LIBRARIES})

target_compile_features(main PUBLIC cxx_std_17)
