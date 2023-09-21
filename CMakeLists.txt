cmake_minimum_required(VERSION 3.26)
project(ffmpeg_code)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(./libs/sdl)

add_library(ffmpeg_code SHARED src/library.cpp
)

target_link_libraries(ffmpeg_code sdl)