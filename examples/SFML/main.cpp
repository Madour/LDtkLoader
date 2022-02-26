// Created by Modar Nasser on 12/11/2020.

#include <SFML/Graphics.hpp>
#include <LDtkLoader/Project.hpp>

#include <iostream>

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

    // load the tileset texture
    sf::Texture tileset_texture;
    tileset_texture.loadFromFile("assets/"+layer.getTileset().path);


    // create the VertexArray that will store all the tiles
    sf::VertexArray tilemap;
    tilemap.resize(tiles_vector.size()*4);  // each tile has 4 vertices
    tilemap.setPrimitiveType(sf::PrimitiveType::Quads);
    int i = 0;
    for (const auto& tile : tiles_vector) {
        auto vertices = tile.getVertices();
        for (int j = 0; j < 4 ; ++j) {
            // set the position and texture coordinate of each vertex
            // flips are already done, you don't have to do it manually
            tilemap[i*4+j].position.x = vertices[j].pos.x;
            tilemap[i*4+j].position.y = vertices[j].pos.y;
            tilemap[i*4+j].texCoords.x = static_cast<float>(vertices[j].tex.x);
            tilemap[i*4+j].texCoords.y = static_cast<float>(vertices[j].tex.y);
        }
        i++;
    }

    // create the window and start the game loop
    sf::RenderWindow window;
    window.create(sf::VideoMode(level.size.x*4, level.size.y*4), "LDtkLoader - SFML");
    window.setFramerateLimit(60);

    sf::Event event{};
    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        // draw the tile map scaled x4
        sf::RenderStates states;
        states.transform.scale(4, 4);
        states.texture = &tileset_texture;
        window.draw(tilemap, states);

        window.display();
    }

    return 0;
}
