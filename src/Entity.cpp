// Created by Modar Nasser on 22/11/2020.

#include <utility>
#include <memory>

#include "LDtkLoader/Entity.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Entity::Entity(const nlohmann::json& j, const World* w) :
name(j["__identifier"].get<std::string>()),
position( j["px"][0].get<unsigned int>(), j["px"][1].get<unsigned int>() ),
grid_pos( j["__grid"][0].get<unsigned int>(), j["__grid"][1].get<unsigned int>() ),
{
    for (const auto& field : j["fieldInstances"]) {
        EntityField f;
        auto&& field_type = field["__type"].get<std::string>();
        // simple fields
        if (field_type == "Int") {
            f.type_name = typeid(int).raw_name();
            f.value = std::make_shared<int>(field["__value"].get<int>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "Float") {
            f.type_name = typeid(float).raw_name();
            f.value = std::make_shared<float>(field["__value"].get<float>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "Bool") {
            f.type_name = typeid(bool).raw_name();
            f.value = std::make_shared<bool>(field["__value"].get<bool>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "String") {
            f.type_name = typeid(std::string).raw_name();
            f.value = std::make_shared<std::string>(field["__value"].get<std::string>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "Color") {
            f.type_name = typeid(Color).raw_name();
            f.value = std::make_shared<std::string>(field["__value"].get<std::string>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type == "Point") {
            f.type_name = typeid(UIntPoint).raw_name();
            f.value = std::make_shared<UIntPoint>(field["__value"]["cx"].get<unsigned int>(), field["__value"]["cy"].get<unsigned int>());
            m_fields[field["__identifier"]] = f;
        }
        else if (field_type.find("LocalEnum") != std::string::npos) {
            auto&& enum_type = field_type.substr(field_type.find('.')+1, field_type.size());
            f.type_name = typeid(EnumValue).raw_name();
            f.value = std::make_shared<EnumValue>(w->getEnum(enum_type)[field["__value"].get<std::string>()]);
            m_fields[field["__identifier"]] = f;
        }

        // array fields
        else if (field_type == "Array<Int>") {
            f.type_name = typeid(int).raw_name();
            for (int v : field["__value"]) {
                f.value = std::make_shared<int>(v);
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        else if (field_type == "Array<Float>") {
            f.type_name = typeid(float).raw_name();
            for (const float v : field["__value"]) {
                f.value = std::make_shared<float>(v);
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        else if (field_type == "Array<Bool>") {
            f.type_name = typeid(bool).raw_name();
            for (const bool v : field["__value"]) {
                f.value = std::make_shared<bool>(v);
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        else if (field_type == "Array<String>") {
            f.type_name = typeid(std::string).raw_name();
            for (const std::string v : field["__value"]) {
                f.value = std::make_shared<std::string>(v);
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        else if (field_type == "Array<Color>") {
            f.type_name = typeid(Color).raw_name();
            for (const std::string v : field["__value"]) {
                f.value = std::make_shared<Color>(v);
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        else if (field_type == "Array<Point>") {
            f.type_name = typeid(UIntPoint).raw_name();
            for (const auto& v : field["__value"]) {
                f.value = std::make_shared<UIntPoint>(v["cx"].get<unsigned int>(), v["cy"].get<unsigned int>());
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        else if (field_type.find("LocalEnum") != std::string::npos) {
            auto&& enum_type = field_type.substr(field_type.find('.')+1, field_type.size());
            f.type_name = typeid(EnumValue).raw_name();
            f.value = std::make_shared<EnumValue>(w->getEnum(enum_type)[field["__value"].get<std::string>()]);
            m_array_fields[field["__identifier"]].push_back(f);
        }
    }
}
