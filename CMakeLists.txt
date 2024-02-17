cmake_minimum_required(VERSION 3.27)
project(SDLGame C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(Libraries/SDL-FontCache)

add_executable(SDL2Test main.c main.h
        LTexture.c
        LTexture.h
        LTimer.c
        LTimer.h
        LevelObjects/Player.c
        LevelObjects/Player.h
        LevelObjects/physics.h
        LevelObjects/LCamera.c
        LevelObjects/LCamera.h
        LevelObjects/TileMap.c
        LevelObjects/TileMap.h
        MenuObjects/LButton.c
        MenuObjects/LButton.h
        MenuObjects/LMenu.c
        MenuObjects/LMenu.h
        LevelObjects/Level.c
        LevelObjects/Level.h
        MenuObjects/Inventory.c
        MenuObjects/Inventory.h
)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})

target_link_libraries(SDL2Test ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} FontCache)