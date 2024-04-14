// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/Level.hpp"

#include <fstream>
#include <memory>

#include "LDtkLoader/World.hpp"

#include "json.hpp"

using namespace ldtk;

Level::Level(const nlohmann::json& j, World* w)
: FieldsContainer(j["fieldInstances"], w)
, world(w)
, name(j["identifier"].get<std::string>())
, iid(j.contains("iid") ? j["iid"].get<std::string>() : "")
, uid(j["uid"].get<int>())
, size(j["pxWid"].get<int>(), j["pxHei"].get<int>())
, position(j["worldX"].get<int>(), j["worldY"].get<int>())
, bg_color(j["__bgColor"].get<std::string>())
, depth(j.contains("worldDepth") ? j["worldDepth"].get<int>() : 0)
{
    m_layers.reserve(j["layerInstances"].size());
    for (const auto& level : j["layerInstances"]) {
        m_layers.emplace_back(level, w, this);
    }

    m_neighbours_id.emplace(Dir::None, 0);
    m_neighbours_id.emplace(Dir::North, 0);
    m_neighbours_id.emplace(Dir::NorthEast, 0);
    m_neighbours_id.emplace(Dir::East, 0);
    m_neighbours_id.emplace(Dir::SouthEast, 0);
    m_neighbours_id.emplace(Dir::South, 0);
    m_neighbours_id.emplace(Dir::SouthWest, 0);
    m_neighbours_id.emplace(Dir::West, 0);
    m_neighbours_id.emplace(Dir::NorthWest, 0);
    m_neighbours_id.emplace(Dir::Over, 0);
    m_neighbours_id.emplace(Dir::Under, 0);
    m_neighbours_id.emplace(Dir::Overlap, 0);
    for (const auto& neighbour : j["__neighbours"]) {
        const auto& dir = neighbour["dir"].get<std::string>();
        const auto& level_iid = IID(neighbour["levelIid"].get<std::string>());

        Dir direction = Dir::None;
        if (dir == "n") {
            direction = Dir::North;
        }
        else if (dir == "ne") {
            direction = Dir::NorthEast;
        }
        else if (dir == "e") {
            direction = Dir::East;
        }
        else if (dir == "se") {
            direction = Dir::SouthEast;
        }
        else if (dir == "s") {
            direction = Dir::South;
        }
        else if (dir == "sw") {
            direction = Dir::SouthWest;
        }
        else if (dir == "w") {
            direction = Dir::West;
        }
        else if (dir == "nw") {
            direction = Dir::NorthWest;
        }
        else if (dir == "o") {
            direction = Dir::Overlap;
        }
        else if (dir == ">") {
            direction = Dir::Over;
        }
        else if (dir == "<") {
            direction = Dir::Under;
        }

        m_neighbours_id[direction].push_back(level_iid);
    }

    if (j["bgRelPath"].is_null()) {
        m_bg_image = null;
    }
    else {
        m_bg_image = BgImage();
        m_bg_image->path = FilePath(j["bgRelPath"].get<std::string>());
        m_bg_image->pos = {j["__bgPos"]["topLeftPx"][0].get<int>(), j["__bgPos"]["topLeftPx"][1].get<int>()};
        m_bg_image->scale = {j["__bgPos"]["scale"][0].get<float>(), j["__bgPos"]["scale"][1].get<float>()};
        m_bg_image->crop = {
            j["__bgPos"]["cropRect"][0].get<int>(),
            j["__bgPos"]["cropRect"][1].get<int>(),
            j["__bgPos"]["cropRect"][2].get<int>(),
            j["__bgPos"]["cropRect"][3].get<int>()
        };
    }
}

auto Level::allLayers() const -> const std::vector<Layer>&
{
    return m_layers;
}

auto Level::getLayer(const std::string& layer_name) const -> const Layer&
{
    for (const auto& layer : m_layers) {
        if (layer.getName() == layer_name) {
            return layer;
        }
    }
    ldtk_error("Layer name \"" + layer_name + "\" not found in Level \"" + name + "\".");
}

auto Level::getLayer(const IID& layer_iid) const -> const Layer&
{
    for (const auto& layer : m_layers) {
        if (layer.iid == layer_iid) {
            return layer;
        }
    }
    ldtk_error("Layer with IID \"" + layer_iid.str() + "\" not found in Level \"" + name + "\".");
}

auto Level::hasBgImage() const -> bool
{
    return m_bg_image.has_value();
}

auto Level::getBgImage() const -> const BgImage&
{
    return m_bg_image.value();
}

auto Level::getNeighbours(const Dir& direction) const -> const std::vector<const Level*>&
{
    return m_neighbours.at(direction);
}

auto Level::getNeighbourDirection(const Level& level) const -> Dir
{
    for (const auto& item : m_neighbours_id) {
        const auto& neighbour_direction = item.first;
        for (const auto& neighbour_iid : item.second) {
            if (neighbour_iid == level.iid) {
                return neighbour_direction;
            }
        }
    }
    return Dir::None;
}
