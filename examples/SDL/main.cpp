// Created by Modar Nasser on 12/11/2020.

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <LDtkLoader/World.hpp>

int main() {
    // declare a LDtk World
    ldtk::World world;

    // load the LDtk World from file
    try {
        world.loadFromFile("assets/level.ldtk");
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // get the level and the layer we want to render
    const auto& level = world.getLevel("Level");
    const auto& layer = level.getLayer("Ground");
    // get all the tiles in the Ground layer
    const auto& tiles_vector = layer.allTiles();

    // Init SDL and create window and Renderer
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_CreateWindowAndRenderer(level.size.x*4, level.size.y*4, SDL_WINDOW_OPENGL, &window, &renderer);
    SDL_SetWindowTitle(window, "LDtkLoader - SDL");

    // load the tileset texture
    SDL_Texture* tileset_texture = IMG_LoadTexture(renderer, ("assets/"+layer.getTileset().path).c_str());

    // create a render texture to draw the map
    SDL_Texture* tilemap_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,   // color format (red, green, blue, alpha)
            SDL_TEXTUREACCESS_TARGET,   // we want to draw things on this texture
            level.size.x,
            level.size.y
    );

    // we start drawing on tilemap_texture
    SDL_SetRenderTarget(renderer, tilemap_texture);
    for (const auto& tile : tiles_vector) {
        // destination rect on the window
        SDL_Rect dest = {
            tile.position.x, tile.position.y,
            layer.getCellSize(), layer.getCellSize()
        };
        // source rect from the tileset
        SDL_Rect src = {
            tile.texture_position.x, tile.texture_position.y,
            layer.getTileset().tile_size, layer.getTileset().tile_size
        };
        // get tile flips
        int flip = (tile.flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)
                 | (tile.flipY ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);

        // draw the tile on the tilemap_texture
        // (0 means no rotation; nullptr sets the rotation pivot to the center of the tile)
        SDL_RenderCopyEx(renderer, tileset_texture, &src, &dest, 0, nullptr, (SDL_RendererFlip)flip);
    }
    // finished drawing on tilemap_texture
    SDL_SetRenderTarget(renderer, nullptr);

    // start the game loop
    bool quit = false;
    SDL_Event event;
    while(!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        unsigned int start = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw the tilemap, fullscreen
        SDL_RenderCopy(renderer, tilemap_texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);

        // cap FPS at 60
        unsigned int end = SDL_GetPerformanceCounter();
        float elapsed_ms = float(end - start)*1000.f / (float)SDL_GetPerformanceFrequency();
        if (elapsed_ms < 16.666)
            SDL_Delay(16.666 - elapsed_ms);
    }

    SDL_DestroyTexture(tileset_texture);
    SDL_DestroyTexture(tilemap_texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
