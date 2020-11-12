// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Level::Level(const nlohmann::json& j, World* w) :
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<unsigned int>()),
size({j["pxWid"].get<unsigned int>(), j["pxHei"].get<unsigned int>()})
{
    for (const auto& level : j["layerInstances"]) {
        Layer new_layer{level};

        const auto& layer_def = w->getLayerDef(new_layer.name);
        new_layer.setLayerDef(layer_def);
        if (new_layer.type == LayerType::AutoLayer || new_layer.type == LayerType::IntGrid)
            if (layer_def.autotileset_id >= 0)
                new_layer.setTileset(w->getTileset(layer_def.autotileset_id));

        if (new_layer.type == LayerType::Tiles)
            if (layer_def.tileset_id >= 0)
                new_layer.setTileset(w->getTileset(layer_def.tileset_id));

        m_layers.push_back(new_layer);
    }
}

auto Level::allLayers() const -> const std::vector<Layer>& {
    return m_layers;
}
