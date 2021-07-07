// Created by Modar Nasser on 13/11/2020.

#pragma once

#include <string>
#include <vector>
#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {

    class World;
    class Enum;
    struct Tileset;

    struct EnumValue {
        const std::string name;
        const Color color;
        const Enum& type;

        auto hasIcon() const -> bool;
        auto getIconTileset() const -> const Tileset&;
        auto getIconTexturePos() const -> IntPoint;

    private:
        friend Enum;
        friend bool operator==(const EnumValue& l, const EnumValue& r);
        EnumValue(std::string name, int id, int tile_id, const Color& color, const Enum& enum_type);
        const int id;
        const int tile_id;
    };

    bool operator==(const EnumValue& l, const EnumValue& r);
    bool operator!=(const EnumValue& l, const EnumValue& r);

    class Enum {
        friend World;

    public:
        Enum(const Enum&) = delete;
        Enum(Enum&&) = default;
        auto operator=(const Enum&) -> Enum& = delete;

        const std::string name;
        const int uid;

        auto operator[](const std::string& val_name) const -> const EnumValue&;

        auto hasIcons() const -> bool;
        auto getIconsTileset() const -> const Tileset&;

        Enum(const nlohmann::json& j, const World* w);

    private:
        const int m_tileset_id;
        const Tileset* m_tileset;
        std::unordered_map<std::string, EnumValue> m_values;
    };

}

auto operator<<(std::ostream& os, const ldtk::EnumValue& enum_value) -> std::ostream&;
