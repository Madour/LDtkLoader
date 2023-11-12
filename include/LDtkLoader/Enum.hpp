// Created by Modar Nasser on 13/11/2020.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "LDtkLoader/thirdparty/json_fwd.hpp"
#include "LDtkLoader/containers/TagsContainer.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {

    class Enum;
    class Tileset;

    struct EnumValue {
        const std::string name;
        const Color color;
        const Enum& type;

        auto hasIcon() const -> bool;
        auto getIconTileset() const -> const Tileset&;
        auto getIconTextureRect() const -> const IntRect&;

    private:
        friend Enum;
        friend auto operator==(const EnumValue& l, const EnumValue& r) -> bool;
        EnumValue(std::string name, int id, const IntRect& tile_rect, const Color& color, const Enum& enum_type);
        const int id;
        const IntRect tile_rect;
    };

    auto operator==(const EnumValue& l, const EnumValue& r) -> bool;
    auto operator!=(const EnumValue& l, const EnumValue& r) -> bool;

    class Enum : public TagsContainer {
        friend class Project;

    public:
        Enum(const Enum&) = delete;
        Enum(Enum&&) = default;
        auto operator=(const Enum&) -> Enum& = delete;

        const std::string name;
        const int uid;

        auto operator[](const std::string& val_name) const -> const EnumValue&;

        auto hasIcons() const -> bool;
        auto getIconsTileset() const -> const Tileset&;

        explicit Enum(const nlohmann::json& j);

    private:
        const int m_tileset_id;
        const Tileset* m_tileset = nullptr;
        std::unordered_map<std::string, EnumValue> m_values;
    };

}

auto operator<<(std::ostream& os, const ldtk::EnumValue& enum_value) -> std::ostream&;
