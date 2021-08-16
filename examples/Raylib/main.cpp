#include <iostream>
#include <LDtkLoader/World.hpp>
#include <raylib.h>

int main()
{
    // Declare a LDtk World.
    ldtk::World world;

    // Load the LDtk World from file.
    try
    {
        world.loadFromFile("assets/level.ldtk");
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return (1);
    }

    // Get the level and the layer we want to render.
    const auto &level = world.getLevel("Level");
    const auto &layer = level.getLayer("Ground");

    // Get all the tiles in the Ground layer.
    const auto &tiles_vector = layer.allTiles();

    // Init Raylib and create a window.
    InitWindow(level.size.x * 4, level.size.y * 4, "LDtkLoader - Raylib");
    SetTargetFPS(30);

    // Load the texture and the renderer.
    Texture2D texture = LoadTexture(("assets/" + layer.getTileset().path).c_str());
    RenderTexture2D renderer = LoadRenderTexture(level.size.x, level.size.y);
    while (!WindowShouldClose())
    {
        // Draw all the tiles.
        BeginTextureMode(renderer);
        ClearBackground(BLACK);
        for (const auto &tile : tiles_vector)
        {
            Vector2 dest = {
                static_cast<float>(tile.position.x),
                static_cast<float>(tile.position.y),
            };
            Rectangle src = {
                static_cast<float>(tile.texture_position.x),
                static_cast<float>(tile.texture_position.y),
                layer.getTileset().tile_size * ((tile.flipX == true) ? -1.0f : 1.0f),
                layer.getTileset().tile_size * ((tile.flipY == true) ? -1.0f : 1.0f)
            };
            DrawTextureRec(texture, src, dest, WHITE);
        }
        EndTextureMode();

        // Scale up the Renderer times 4.
        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
        BeginDrawing();
        Rectangle src = {
            0,
            0,
            static_cast<float>(renderer.texture.width),
            -static_cast<float>(renderer.texture.height)
        };
        Rectangle dest = {
            0,
            0,
            static_cast<float>(renderer.texture.width) * 4,
            static_cast<float>(renderer.texture.height) * 4
        };
        DrawTexturePro(renderer.texture, src, dest, {0}, .0f, WHITE);
        EndDrawing();
    }

    // Unload everything before leaving.
    UnloadRenderTexture(renderer);
    UnloadTexture(texture);
    CloseWindow();
    return (0);
}
