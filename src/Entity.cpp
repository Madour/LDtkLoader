// Created by Modar Nasser on 22/11/2020.

#include "LDtkLoader/Entity.hpp"

using namespace ldtk;

Entity::Entity(const nlohmann::json& j) :
name(j["__identifier"].get<std::string>()),
position( {j["px"][0].get<unsigned int>(), j["px"][1].get<unsigned int>()} ),
grid_pos( {j["__grid"][0].get<unsigned int>(), j["__grid"][1].get<unsigned int>()} )
{
    for (const auto& field : j["fieldInstances"]) {
        EntityField f;
        // simple fields
        if (field["__type"] == "Int") {
            f.type_name = typeid(int).raw_name();
            f.value = std::shared_ptr<void>(new int(field["__value"].get<int>()));
            m_fields[field["__identifier"]] = f;
        }
        if (field["__type"] == "Float") {
            f.type_name = typeid(float).raw_name();
            f.value = std::shared_ptr<void>(new float(field["__value"].get<float>()));
            m_fields[field["__identifier"]] = f;
        }
        if (field["__type"] == "Bool") {
            f.type_name = typeid(bool).raw_name();
            f.value = std::shared_ptr<void>(new bool(field["__value"].get<bool>()));
            m_fields[field["__identifier"]] = f;
        }
        if (field["__type"] == "String") {
            f.type_name = typeid(std::string).raw_name();
            f.value = std::shared_ptr<void>(new std::string(field["__value"].get<std::string>()));
            m_fields[field["__identifier"]] = f;
        }
        if (field["__type"] == "Color") {
            f.type_name = typeid(Color).raw_name();
            f.value = std::shared_ptr<void>(new Color(field["__value"].get<std::string>()));
            m_fields[field["__identifier"]] = f;
        }
        if (field["__type"] == "Point") {
            f.type_name = typeid(UIntPoint).raw_name();
            f.value = std::shared_ptr<void>(new UIntPoint({field["__value"]["cx"].get<unsigned int>(), field["__value"]["cy"].get<unsigned int>()}));
            m_fields[field["__identifier"]] = f;
        }

        // array fields
        if (field["__type"] == "Array<Int>") {
            f.type_name = typeid(int).raw_name();
            for (int v : field["__value"]) {
                f.value = std::shared_ptr<void>(new int(v));
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        if (field["__type"] == "Array<Float>") {
            f.type_name = typeid(float).raw_name();
            for (float v : field["__value"]) {
                f.value = std::shared_ptr<void>(new float(v));
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        if (field["__type"] == "Array<Bool>") {
            f.type_name = typeid(bool).raw_name();
            for (bool v : field["__value"]) {
                f.value = std::shared_ptr<void>(new bool(v));
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        if (field["__type"] == "Array<String>") {
            f.type_name = typeid(std::string).raw_name();
            for (std::string v : field["__value"]) {
                f.value = std::shared_ptr<void>(new std::string(v));
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        if (field["__type"] == "Array<Color>") {
            f.type_name = typeid(Color).raw_name();
            for (std::string v : field["__value"]) {
                f.value = std::shared_ptr<void>(new Color(v));
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
        if (field["__type"] == "Array<Point>") {
            f.type_name = typeid(UIntPoint).raw_name();
            for (const auto& v : field["__value"]) {
                f.value = std::shared_ptr<void>(new UIntPoint({v["cx"].get<unsigned int>(), v["cy"].get<unsigned int>()}));
                m_array_fields[field["__identifier"]].push_back(f);
            }
        }
    }
}
