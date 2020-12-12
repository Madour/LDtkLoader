// Created by Modar Nasser on 22/11/2020.

#include <memory>

#include "LDtkLoader/Entity.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Entity::Entity(const nlohmann::json& j, const World* w) :
m_definition( &w->getEntityDef(j["defUid"].get<int>()) ),
m_position( j["px"][0].get<int>(), j["px"][1].get<int>() ),
m_grid_pos( j["__grid"][0].get<int>(), j["__grid"][1].get<int>() )
{
    for (const auto& field : j["fieldInstances"]) {
        EntityField f;
        auto&& field_type = field["__type"].get<std::string>();

        // array fields
        if (field_type.find("Array") != std::string::npos) {
            m_array_fields[field["__identifier"]] = {};
            if (field_type == "Array<Int>") {
                f.type_name = typeid(int).name();
                for (int v : field["__value"]) {
                    f.value = std::make_shared<int>(v);
                    m_array_fields[field["__identifier"]].push_back(f);
                }
            }
            else if (field_type == "Array<Float>") {
                f.type_name = typeid(float).name();
                for (const float v : field["__value"]) {
                    f.value = std::make_shared<float>(v);
                    m_array_fields[field["__identifier"]].push_back(f);
                }
            }
            else if (field_type == "Array<Bool>") {
                f.type_name = typeid(bool).name();
                for (const bool v : field["__value"]) {
                    f.value = std::make_shared<bool>(v);
                    m_array_fields[field["__identifier"]].push_back(f);
                }
            }
            else if (field_type == "Array<String>") {
                f.type_name = typeid(std::string).name();
                for (const std::string v : field["__value"]) {
                    f.value = std::make_shared<std::string>(v);
                    m_array_fields[field["__identifier"]].push_back(f);
                }
            }
            else if (field_type == "Array<Color>") {
                f.type_name = typeid(Color).name();
                for (const std::string v : field["__value"]) {
                    f.value = std::make_shared<Color>(v);
                    m_array_fields[field["__identifier"]].push_back(f);
                }
            }
            else if (field_type == "Array<Point>") {
                f.type_name = typeid(IntPoint).name();
                for (const auto& v : field["__value"]) {
                    f.value = std::make_shared<IntPoint>(v["cx"].get<int>(), v["cy"].get<int>());
                    m_array_fields[field["__identifier"]].push_back(f);
                }
            }
            else if (field_type.find("LocalEnum") != std::string::npos) {
                auto&& enum_type = field_type.substr(field_type.find('.')+1, field_type.size()-field_type.find('.')-2);
                f.type_name = typeid(EnumValue).name();
                for (const auto& v : field["__value"]) {
                    if (v.is_null())
                        f.value = std::make_shared<EnumValue>(EnumValue::None);
                    else
                        f.value = std::make_shared<EnumValue>(w->getEnum(enum_type)[v.get<std::string>()]);
                    m_array_fields[field["__identifier"]].push_back(f);
                }
            }
        }
        // simple fields
        else if (field_type == "Int") {
            f.type_name = typeid(int).name();
            f.value = std::make_shared<int>(field["__value"].get<int>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "Float") {
            f.type_name = typeid(float).name();
            f.value = std::make_shared<float>(field["__value"].get<float>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "Bool") {
            f.type_name = typeid(bool).name();
            f.value = std::make_shared<bool>(field["__value"].get<bool>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "String") {
            f.type_name = typeid(std::string).name();
            f.value = std::make_shared<std::string>(field["__value"].get<std::string>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "Color") {
            f.type_name = typeid(Color).name();
            f.value = std::make_shared<std::string>(field["__value"].get<std::string>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "Point") {
            f.type_name = typeid(IntPoint).name();
            f.value = std::make_shared<IntPoint>(field["__value"]["cx"].get<int>(), field["__value"]["cy"].get<int>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type.find("LocalEnum") != std::string::npos) {
            auto&& enum_type = field_type.substr(field_type.find('.')+1, field_type.size());
            f.type_name = typeid(EnumValue).name();
            if (field["__value"].is_null())
                f.value = std::make_shared<EnumValue>(EnumValue::None);
            else
                f.value = std::make_shared<EnumValue>(w->getEnum(enum_type)[field["__value"].get<std::string>()]);
            m_fields[field["__identifier"]] = f;
        }
    }
}

auto Entity::getName() const -> const std::string& {
    return m_definition->name;
}

auto Entity::getSize() const -> const IntPoint& {
    return m_definition->size;
}

auto Entity::getPosition() const -> const IntPoint& {
    return m_position;
}

auto Entity::getGridPosition() const -> const IntPoint& {
    return m_grid_pos;
}

auto Entity::getColor() const -> const Color& {
    return m_definition->color;
}

auto Entity::getPivot() const -> const FloatPoint& {
    return m_definition->pivot;
}
