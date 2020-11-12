// Created by Modar Nasser on 11/11/2020.

#include "LDtkLoader/World.hpp"

int main() {
    ldtk::World world;
    world.loadFromFile("level.ldtk");
    return 0;
}