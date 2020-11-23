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
        Layer new_layer{level, w};
        m_layers.push_back(std::move(new_layer));
    }
}

Level::Level(Level&& other) noexcept :
name(other.name),
uid(other.uid),
size(other.size),
m_layers(std::move(other.m_layers))
{}

auto Level::allLayers() const -> const std::vector<Layer>& {
    return m_layers;
}

auto Level::getLayer(const std::string& layer_name) const -> const Layer& {
    for (const auto& layer : m_layers)
        if (layer.name == layer_name)
            return layer;
    throw std::invalid_argument("Layer name "+layer_name+" not found in Level "+layer_name);
}
