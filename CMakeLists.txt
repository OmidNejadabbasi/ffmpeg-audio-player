cmake_minimum_required(VERSION 3.26)
project(ffmpeg_code)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_subdirectory(./libs/sdl EXCLUDE_FROM_ALL)
add_executable(ffmpeg_code src/main.cpp
        src/library.cpp)

find_package(FFMPEG REQUIRED)
target_include_directories(ffmpeg_code PRIVATE ${FFMPEG_INCLUDE_DIRS} SDL2::SDL2)
target_link_directories(ffmpeg_code PRIVATE ${FFMPEG_LIBRARY_DIRS} SDL2::SDL2)
target_link_libraries(ffmpeg_code PRIVATE ${FFMPEG_LIBRARIES} SDL2::SDL2)
