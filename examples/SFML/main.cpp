// Created by Modar Nasser on 12/11/2020.

#include <SFML/Graphics.hpp>
#include <LDtkLoader/World.hpp>

int main() {

    ldtk::World world;
    world.loadFromFile("level.ldtk");

    sf::Texture tileset_texture;
    tileset_texture.loadFromFile(world.getTileset("Tileset").path);

    const auto& tiles_vector = world.allLevels()[0].allLayers()[0]->allTiles();

    sf::RenderWindow window;
    window.create(sf::VideoMode(world.allLevels()[0].size.x*4, world.allLevels()[0].size.y*4), "Title");
    window.setFramerateLimit(60);

    sf::VertexArray tilemap;
    tilemap.resize(tiles_vector.size()*4);
    tilemap.setPrimitiveType(sf::PrimitiveType::Quads);

    int i = 0;
    for (const auto& tile : tiles_vector) {
        for (int j = 0; j < 4 ; ++j) {
            tilemap[i*4+j].position.x = tile->vertices[j].pos.x;
            tilemap[i*4+j].position.y = tile->vertices[j].pos.y;
            tilemap[i*4+j].texCoords.x = tile->vertices[j].tex.x;
            tilemap[i*4+j].texCoords.y = tile->vertices[j].tex.y;
        }
        i++;
    }

    sf::Event event{};
    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        sf::RenderStates states;
        states.transform.scale(4, 4);
        states.texture = &tileset_texture;
        window.draw(tilemap, states);

        window.display();
    }

    return 0;
}