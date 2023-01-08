// Created by Modar Nasser on 22/11/2020.

#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "LDtkLoader/thirdparty/json_fwd.hpp"
#include "LDtkLoader/containers/FieldsContainer.hpp"
#include "LDtkLoader/defs/EntityDef.hpp"
#include "LDtkLoader/defs/FieldDef.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk {

    class World;

    class Entity : public FieldsContainer {
    public:
        Entity(const Entity&) = delete;
        Entity(Entity&&) = default;
        auto operator=(const Entity&) -> Entity& = delete;

        const IID iid;

        auto getName() const -> const std::string&;
        auto getSize() const -> const IntPoint&;
        auto getPosition() const -> const IntPoint&;
        auto getGridPosition() const -> const IntPoint&;
        auto getColor() const -> const Color&;
        auto getPivot() const -> const FloatPoint&;

        auto hasSprite() const -> bool;
        auto getTexturePath() const -> const std::string&;
        auto getTextureRect() const -> const IntRect&;

        auto hasNineSlice() const -> bool;
        auto getNineSliceBorders() const -> const NineSliceBorders&;

        auto hasTag(const std::string& tag) const -> bool;
        auto getTags() const -> const std::vector<std::string>&;

        auto allFields() const -> const std::vector<FieldDef>&;

        explicit Entity(const nlohmann::json& j, const World* w);

    private:
        const EntityDef* m_definition = nullptr;

        const IntPoint m_size;
        const IntPoint m_position;
        const IntPoint m_grid_pos;
        const Color m_color;

        const Tileset* m_tileset = nullptr;
        const IntRect m_texture_rect;
    };

}
