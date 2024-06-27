cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
project(sdl_test CXX)

add_executable(${PROJECT_NAME} main.cpp)


find_package(SDL2)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
