cmake_minimum_required(VERSION 3.20)
project(game-of-life)

add_executable(game-of-life src/game-of-life.c)

target_sources(game-of-life PRIVATE src/game-of-life.c)

add_subdirectory(include/raylib/)
target_link_libraries(game-of-life PUBLIC raylib)
target_include_directories(game-of-life PUBLIC "src" "include/raylib")
