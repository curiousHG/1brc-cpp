cmake_minimum_required(VERSION 3.20)
project(onebrc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
add_compile_options(-Wall -Wextra)

find_package(Threads REQUIRED)

add_executable(calculate_average src/main.cpp)
target_link_libraries(calculate_average PRIVATE Threads::Threads)

add_executable(create_measurements tools/create_measurements.cpp)
