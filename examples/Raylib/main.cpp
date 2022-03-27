#include <iostream>
#include <LDtkLoader/Project.hpp>
#include <raylib.h>

int main() {
    // declare a LDtk World
    ldtk::Project ldtk_project;

    // load the LDtk World from file
    try {
        ldtk_project.loadFromFile("assets/level.ldtk");
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // get the world
    const auto& world = ldtk_project.getWorld();

    // get the level and the layer we want to render
    const auto& level = world.getLevel("Level");
    const auto& layer = level.getLayer("Ground");
    // get all the tiles in the Ground layer
    const auto& tiles_vector = layer.allTiles();

    // Init Raylib and create a window.
    InitWindow(level.size.x * 4, level.size.y * 4, "LDtkLoader - Raylib");
    SetTargetFPS(30);

    // Load the texture and the renderer.
    Texture2D texture = LoadTexture(("assets/" + layer.getTileset().path).c_str());
    RenderTexture2D renderer = LoadRenderTexture(level.size.x, level.size.y);

    // Draw all the tiles.
    BeginTextureMode(renderer);
    ClearBackground(BLACK);
    for (const auto &tile : tiles_vector) {
        const auto& position = tile.getPosition();
        const auto& texture_rect = tile.getTextureRect();
        Vector2 dest = {
            static_cast<float>(position.x),
            static_cast<float>(position.y),
        };
        Rectangle src = {
            static_cast<float>(texture_rect.x),
            static_cast<float>(texture_rect.y),
            static_cast<float>(texture_rect.width) * (tile.flipX ? -1.0f : 1.0f),
            static_cast<float>(texture_rect.height) * (tile.flipY ? -1.0f : 1.0f)
        };
        DrawTextureRec(texture, src, dest, WHITE);
    }
    EndTextureMode();

    while (!WindowShouldClose()) {
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
