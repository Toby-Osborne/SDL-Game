# Fatformer
_A platforming game about a fat man trying to lose weight_

To be honest, this is more of a game engine at the moment.

## Building with CMake
### `FontCacheCMakeLists.patch`
- I have not forked the FontCache for my installs, so after initializing the submodule, apply the patch to the `Libraries/SDL-FontCache/CMakeLists.txt`

### Linux Mint / Ubuntu
```
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-ttf-dev
```
It will now just work
### Windows
1. Download the mingw32 version of SDL2, SDL2_image, SDL2_ttf, `SDL2-devel-x-mingw.zip`
2. Unzip all into the same directory.  The `cmake`, `i686w64-mingw32`, `x86_64-w64-mingw32` directories should all be combined
3. Reset cmake and load the project, then build.
4. Drag and Drop the relevant dll files (found in `SDL2/x86_64-w64-mingw32/bin`) into cmake-build-debug, or wherever you're building the game.

## TODO:
### Bugs:
 - Cannot select first tile in level edit
 - Tile-mapTiles is drawn with 500 GPU calls, not scalable to multiple layers or viewports

### Features:
 - HUD for inventory
 - Options menu
   - Resolution
   - GUI Scale
 - Resizable windows
 - Use Viewport to implement batch rendering of tile mapTiles
 - Jump height varies with how long you press UP
 - CTRL-Z for mapTiles edit, block-pick key
 - Rework physics engine to work with signed integers and bit shift them by 1024 on exit.