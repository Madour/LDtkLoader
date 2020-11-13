// Created by Modar Nasser on 12/11/2020.

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <LDtkLoader/World.hpp>

int main() {

    // load the LDtk file
    ldtk::World world;
    world.loadFromFile("level.ldtk");

    // get the level and the layer we want to render
    const auto& level = world.getLevel("Level");
    const auto& layer = level.getLayer("Ground");
    // get all the tiles in the Ground layer
    const auto& tiles_vector = layer.allTiles();

    // Init SDL and create window and Renderer
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_CreateWindowAndRenderer(int(level.size.x)*4, int(level.size.y)*4, SDL_WINDOW_OPENGL, &window, &renderer);
    SDL_SetWindowTitle(window, "LDtkLoader - SDL");

    // load the tileset texture
    SDL_Texture* tileset_texture = IMG_LoadTexture(renderer, layer.getTileset().path.c_str());

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
                int(tile.position.x), int(tile.position.y),
                int(layer.cell_size), int(layer.cell_size)
        };
        // source rect from the tileset
        SDL_Rect src = {
                int(tile.texture_position.x), int(tile.texture_position.y),
                int(layer.getTileset().tile_size), int(layer.getTileset().tile_size)
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

    int quit = 0;
    SDL_Event event;
    while(!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
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
        float elapsed_ms = float(end - start) / (float)SDL_GetPerformanceFrequency();
        SDL_Delay(floor(16.666 - elapsed_ms));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_DestroyTexture(tileset_texture);
    SDL_DestroyTexture(tilemap_texture);
    SDL_Quit();

    return 0;
}
