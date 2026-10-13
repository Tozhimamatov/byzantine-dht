cmake_minimum_required(VERSION 3.20)
project(ByzantineDHT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dht_core INTERFACE)
target_include_directories(dht_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(dht_demo src/main.cpp)
target_link_libraries(dht_demo PRIVATE dht_core)

# Threading support
find_package(Threads REQUIRED)
target_link_libraries(dht_demo PRIVATE Threads::Threads)

if (MSVC)
    target_compile_options(dht_demo PRIVATE /W4 /O2)
else()
    target_compile_options(dht_demo PRIVATE -Wall -Wextra -Wpedantic -O3)
endif()
