// Created by Modar Nasser on 13/11/2020.

#pragma once

#include <string>
#include <vector>
#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class World;
    class Enum;

    struct EnumValue {
        const std::string name;
    private:
        friend Enum;
        friend bool operator==(const EnumValue& l, const EnumValue& r);
        EnumValue(std::string name, int id, int tile_id, int type_id);
        const int id;
        const int tile_id;
        const int type_id;
    };

    bool operator==(const EnumValue& l, const EnumValue& r);
    bool operator!=(const EnumValue& l, const EnumValue& r);

    class Enum {
        friend World;
    public:
        const std::string name;
        int uid;

        auto operator[](const std::string& val_name) const -> const EnumValue&;

        auto hasIcons() const -> bool;
        auto getIconsTileset() const -> const Tileset&;
        auto getIconTexturePos(const std::string& val_name) const -> IntPoint;
        auto getIconTexturePos(const EnumValue& val) const -> IntPoint;

    private:
        Enum(const nlohmann::json& j, const World* w);

        const Tileset* m_tileset = nullptr;
        std::unordered_map<std::string, EnumValue> m_values;
    };

}

auto operator<<(std::ostream& os, const ldtk::EnumValue& enum_value) -> std::ostream&;
