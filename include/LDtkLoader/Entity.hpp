// Created by Modar Nasser on 22/11/2020.

#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/FieldsContainer.hpp"
#include "LDtkLoader/EntityDef.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk {

    class World;
    class Layer;

    class Entity : public FieldsContainer {
        friend Layer;

    public:
        Entity(const Entity&) = delete;
        Entity(Entity&&) = default;
        auto operator=(const Entity&) -> Entity& = delete;

        auto getName() const -> const std::string&;
        auto getSize() const -> const IntPoint&;
        auto getPosition() const -> const IntPoint&;
        auto getGridPosition() const -> const IntPoint&;
        auto getColor() const -> const Color&;
        auto getPivot() const -> const FloatPoint&;

        auto hasIcon() const -> bool;
        auto getIconTileset() const -> const Tileset&;
        auto getIconTexturePos() const -> IntPoint;

        explicit Entity(const nlohmann::json& j, const World* w);

    private:
        const EntityDef* m_definition = nullptr;

        const IntPoint m_size;
        const IntPoint m_position;
        const IntPoint m_grid_pos;
    };

}
