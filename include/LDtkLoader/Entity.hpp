// Created by Modar Nasser on 22/11/2020.

#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/EntityDef.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk {

    class World;
    class Layer;
    class Entity;

    class EntityField {
        friend Entity;
        const char* type_name;
        std::shared_ptr<void> value;
    };

    class Entity {
        friend Layer;
    public:

        auto getName() const -> const std::string&;
        auto getSize() const -> const IntPoint&;
        auto getPosition() const -> const IntPoint&;
        auto getGridPosition() const -> const IntPoint&;
        auto getColor() const -> const Color&;
        auto getPivot() const -> const FloatPoint&;

        template<typename T>
        auto getField(const std::string& field_name) const -> const T&;

        template<typename T>
        auto getArrayField(const std::string& field_name) const -> std::vector<T>;

    private:
        explicit Entity(const nlohmann::json& j, const World* w);

        const EntityDef* m_definition = nullptr;

        const IntPoint m_position;
        const IntPoint m_grid_pos;

        std::unordered_map<std::string, EntityField> m_fields;
        std::unordered_map<std::string, std::vector<EntityField>> m_array_fields;
    };

    template <typename T>
    auto Entity::getField(const std::string& field_name) const -> const T& {
        if (m_fields.count(field_name) > 0) {
            const auto& field = m_fields.at(field_name);
            if (typeid(T).name() == field.type_name)
                return *static_cast<T*>(field.value.get());
            else
                ldtk_error("Field \""+field_name+"\" of entity \""+getName()+"\" is of type \""+field.type_name+"\" and not \""+typeid(T).name()+"\".");
        }
        if (m_array_fields.count(field_name) > 0)
            ldtk_error("Field \""+field_name+"\" of Entity \""+getName()+"\" is an Array Field, try using `getArrayField`.");
        ldtk_error("Entity \""+getName()+"\" does not have a field name \""+field_name+"\".");
    }

    template <typename T>
    auto Entity::getArrayField(const std::string& field_name) const -> std::vector<T> {
        if (m_array_fields.count(field_name) > 0) {
            const auto& array_field = m_array_fields.at(field_name);
            if (array_field.empty())
                return {};
            if (typeid(T).name() == array_field.at(0).type_name) {
                std::vector<T> res;
                for (const auto& e : array_field) {
                    res.push_back(*static_cast<T*>(e.value.get()));
                }
                return res;
            }
            else
                ldtk_error("Array field \""+field_name+"\" of entity \""+getName()+"\" is of type \""+array_field.at(0).type_name+"\" and not \""+typeid(T).name()+"\".");
        }
        if (m_fields.count(field_name) > 0)
            ldtk_error("Field \""+field_name+"\" of Entity \""+getName()+"\" is not an Array Field, try using `getField`.");
        ldtk_error("Entity \""+getName()+"\" does not have an array field named \""+field_name+"\".");
    }

}
