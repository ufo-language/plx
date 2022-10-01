cmake_minimum_required (VERSION 3.10)
project(plx)

#C_OPT_DEBUG="-O0 -g -ggdb"
#C_OPT_SIZE="-static -Os -ffunction-sections -fdata-sections"
#C_OPT_SPEED="-O2"

#debug
add_compile_options(-g -ggdb -Wall -Wextra -pedantic)

#size
#add_compile_options(-Os -Wall -Wextra -pedantic -ffunction-sections -fdata-sections)
#add_link_options(-Wl,-gc-sections)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCE_FILES src/src/*.cpp)
include_directories(src)

add_library(PLX ${SOURCE_FILES})
add_executable(plx src/main.cpp)
target_link_libraries(plx PLX)

# testing
find_package(Catch2)
file(GLOB TEST_SOURCE_FILES test/*.cpp test/*/*.cpp)
add_executable(test-${PROJECT_NAME} ${TEST_SOURCE_FILES})
target_link_libraries(test-${PROJECT_NAME} PLX Catch2WithMain)
