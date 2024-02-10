cmake_minimum_required(VERSION 3.27)
project(SDLGame C)

set(CMAKE_C_STANDARD 17)

add_executable(SDL2Test main.c main.h
        LTexture.c
        LTexture.h)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

target_link_libraries(SDL2Test ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})