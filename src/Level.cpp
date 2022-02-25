// Created by Modar Nasser on 12/11/2020.

#include <fstream>

#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Level::Level(const nlohmann::json& j, World* w) :
FieldsContainer(j["fieldInstances"], w),
world(w),
name(j["identifier"].get<std::string>()),
iid(j["iid"].get<std::string>()),
uid(j["uid"].get<int>()),
size(j["pxWid"].get<int>(), j["pxHei"].get<int>()),
position(j["worldX"].get<int>(), j["worldY"].get<int>()),
bg_color(j["__bgColor"].get<std::string>())
{
    std::unique_ptr<nlohmann::json> pjl = nullptr;

    // check if level is external and open it if so
    if (!j["externalRelPath"].is_null()) {
        std::ifstream in(w->getFilePath().directory() + j["externalRelPath"].get<std::string>());
        if (in.fail()) {
            ldtk_error("Failed to open file \"" + j["externalRelPath"].get<std::string>() + "\" : " + strerror(errno));
        }
        pjl = std::make_unique<nlohmann::json>();
        in >> *pjl;
    }

    const auto& jl = (pjl == nullptr ? j : *pjl);

    m_layers.reserve(jl["layerInstances"].size());
    for (const auto& level : jl["layerInstances"]) {
        m_layers.emplace_back(level, w, this);
    }

    m_neighbours_id[Dir::North]; m_neighbours_id[Dir::East];
    m_neighbours_id[Dir::South]; m_neighbours_id[Dir::West];
    for (const auto& neighbour : j["__neighbours"]) {
        const auto& dir = neighbour["dir"].get<std::string>();
        const auto& level_uid = neighbour["levelUid"].get<int>();
        if (dir == "n")
            m_neighbours_id[Dir::North].push_back(level_uid);
        else if (dir == "e")
            m_neighbours_id[Dir::East].push_back(level_uid);
        else if (dir == "s")
            m_neighbours_id[Dir::South].push_back(level_uid);
        else
            m_neighbours_id[Dir::West].push_back(level_uid);
    }

    if (j["bgRelPath"].is_null())
        m_bg_image = null;
    else {
        m_bg_image = BgImage();
        m_bg_image->path = FilePath(j["bgRelPath"].get<std::string>());
        m_bg_image->pos = {j["__bgPos"]["topLeftPx"][0].get<int>(), j["__bgPos"]["topLeftPx"][1].get<int>()};
        m_bg_image->scale = {j["__bgPos"]["scale"][0].get<float>(), j["__bgPos"]["scale"][1].get<float>()};
        m_bg_image->crop = {j["__bgPos"]["cropRect"][0].get<int>(), j["__bgPos"]["cropRect"][1].get<int>(),
                            j["__bgPos"]["cropRect"][2].get<int>(), j["__bgPos"]["cropRect"][3].get<int>()};
    }
}

auto Level::allLayers() const -> const std::vector<Layer>& {
    return m_layers;
}

auto Level::getLayer(const std::string& layer_name) const -> const Layer& {
    for (const auto& layer : m_layers)
        if (layer.getName() == layer_name)
            return layer;
    ldtk_error("Layer name \""+layer_name+"\" not found in Level \""+name+"\".");
}

auto Level::hasBgImage() const -> bool {
    return m_bg_image.has_value();
}

auto Level::getBgImage() const -> const BgImage& {
    return m_bg_image.value();
}

auto Level::getNeighbours(const Dir& direction) const -> const std::vector<const Level*>& {
    return m_neighbours.at(direction);
}

auto Level::getNeighbourDirection(const Level& level) const -> Dir {
    for (const auto& item : m_neighbours_id) {
        for (auto id : item.second) {
            if (id == level.uid)
                return item.first;
        }
    }
    return Dir::None;
}
