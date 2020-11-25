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
        auto getSize() const -> const UIntPoint&;
        auto getPosition() const -> const UIntPoint&;
        auto getGridPosition() const -> const UIntPoint&;
        auto getColor() const -> const Color&;
        auto getPivot() const -> const FloatPoint&;

        template<typename T>
        auto getField(const std::string& field_name) const -> const T&;

    private:
        explicit Entity(const nlohmann::json& j, const World* w);

        const EntityDef* m_definition = nullptr;

        const UIntPoint m_position;
        const UIntPoint m_grid_pos;

        std::unordered_map<std::string, EntityField> m_fields;
        std::unordered_map<std::string, std::vector<EntityField>> m_array_fields;
    };

    template <typename T>
    auto Entity::getField(const std::string& field_name) const -> const T& {
        if (m_fields.count(field_name) > 0) {
            if (typeid(T).name() == m_fields.at(field_name).type_name)
                return *static_cast<T*>(m_fields.at(field_name).value.get());
            else {
                std::cerr << "Field " << field_name << " of entity " << getName()
                          << " is not of type " << typeid(T).name() << std::endl;
                throw std::invalid_argument("Field "+field_name+" of entity "+getName()+" is not of type "+typeid(T).name());
            }
        }
        throw std::invalid_argument("Entity "+getName()+" does not have a field name "+field_name);
    }

}
