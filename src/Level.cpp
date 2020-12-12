// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Level::Level(const nlohmann::json& j, World* w) :
world(w),
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
size(j["pxWid"].get<int>(), j["pxHei"].get<int>()),
position(j["worldX"].get<int>(), j["worldY"].get<int>())
{
    for (const auto& level : j["layerInstances"]) {
        Layer new_layer{level, w, this};
        m_layers.push_back(std::move(new_layer));
    }

    m_neighbours[Dir::North]; m_neighbours[Dir::East];
    m_neighbours[Dir::South]; m_neighbours[Dir::West];
    for (const auto& neighbour : j["__neighbours"]) {
        const auto& dir = neighbour["dir"].get<std::string>();
        const auto& level_uid = neighbour["levelUid"].get<int>();
        if (dir == "n")
            m_neighbours[Dir::North].push_back(level_uid);
        else if (dir == "e")
            m_neighbours[Dir::East].push_back(level_uid);
        else if (dir == "s")
            m_neighbours[Dir::South].push_back(level_uid);
        else
            m_neighbours[Dir::West].push_back(level_uid);
    }
}

Level::Level(Level&& other) noexcept :
world(other.world),
name(other.name),
uid(other.uid),
size(other.size),
position(other.position),
m_layers(std::move(other.m_layers)),
m_neighbours(std::move(other.m_neighbours))
{}

auto Level::allLayers() const -> const std::vector<Layer>& {
    return m_layers;
}

auto Level::getLayer(const std::string& layer_name) const -> const Layer& {
    for (const auto& layer : m_layers)
        if (layer.getName() == layer_name)
            return layer;
    ldtk_error("Layer name \""+layer_name+"\" not found in Level \""+name+"\".");
}

auto Level::getNeighbours(const Dir& direction) const -> std::vector<const Level*> {
    std::vector<const Level*> res;
    if (direction != Dir::None)
        for (auto id : m_neighbours.at(direction))
            res.emplace_back(&world->getLevel(id));
    return res;
}

auto Level::getNeighbourDirection(const Level& level) const -> Dir {
    for (const auto& item : m_neighbours) {
        for (auto id : item.second) {
            if (id == level.uid)
                return item.first;
        }
    }
    return Dir::None;
}
