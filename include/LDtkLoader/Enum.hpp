// Created by Modar Nasser on 13/11/2020.

#pragma once

#include <string>
#include <vector>
#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class World;

    struct EnumValue {
        const std::string name;
        const unsigned int id;
        const unsigned int tile_id;
        const unsigned int type_id;
    };

    class Enum {
        friend World;
    public:
        const std::string name;
        unsigned int uid;

        auto operator[](const std::string& val_name) const -> const EnumValue&;

        auto hasIcons() -> bool;
        auto getIconsTileset() -> const Tileset&;
        auto getIconTexturePos(const std::string& val_name) const -> UIntPoint;

    private:
        Enum(const nlohmann::json& j, World* w);

        const Tileset* m_tileset = nullptr;
        std::unordered_map<std::string, EnumValue> m_values;
    };

}
