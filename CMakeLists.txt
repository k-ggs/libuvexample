cmake_minimum_required(VERSION 3.16)

project(libuvexample LANGUAGES C CXX )

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rdpart/libuv)

find_package(GTest CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
add_executable(libuvexample main.cpp)
target_link_libraries(libuvexample 
uv
GTest::gtest
GTest::gtest_main
spdlog::spdlog
)
enable_testing()
