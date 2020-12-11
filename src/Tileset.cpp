// Created by Modar Nasser on 11/11/2020.

#include "LDtkLoader/Tileset.hpp"

using namespace ldtk;

Tileset::Tileset(const nlohmann::json& j) :
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
path(j["relPath"].get<std::string>()),
texture_size({j["pxWid"].get<int>(), j["pxHei"].get<int>()}),
tile_size(j["tileGridSize"].get<int>()),
spacing(j["spacing"].get<int>()),
padding(j["padding"].get<int>())
{}

auto Tileset::getTileTexturePos(int tile_id) const -> IntPoint {
    auto grid_width = texture_size.x / tile_size;
    return {
        padding + (tile_id % grid_width) * (tile_size + spacing),
        padding + (tile_id / grid_width) * (tile_size + spacing)
    };
}
