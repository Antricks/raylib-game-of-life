cmake_minimum_required(VERSION 3.15)
project(game-of-life)

find_package(raylib 3.0 REQUIRED) # Requires at least version 3.0

set(CMAKE_C_STANDARD 11) # Requires C11 standard

add_executable(${PROJECT_NAME} src/game-of-life.c src/data-structure.c)

target_sources(${PROJECT_NAME} PRIVATE src/game-of-life.c)

target_link_libraries(${PROJECT_NAME} raylib)
