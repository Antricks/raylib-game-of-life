cmake_minimum_required(VERSION 3.15)
project(game-of-life)

find_package(raylib 3.0 REQUIRED) # Requires at least version 3.0

set(CMAKE_C_STANDARD 11) # Requires C11 standard

set(GCC_COVERAGE_COMPILE_FLAGS "-g")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(${PROJECT_NAME} src/game-of-life.c src/data-structure.c)

target_sources(${PROJECT_NAME} PRIVATE src/game-of-life.c)

target_link_libraries(${PROJECT_NAME} raylib)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
