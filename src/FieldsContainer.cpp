// Created by Modar Nasser on 11/03/2021.

#include "LDtkLoader/containers/FieldsContainer.hpp"

#include "LDtkLoader/World.hpp"

#include "json.hpp"

using namespace ldtk;

namespace ldtk {
    extern std::vector<EntityRef*> temporary_entity_refs_array;
}

FieldsContainer::FieldsContainer(const nlohmann::json& j, const World* w)
{
    parseFields(j, w);
}

void FieldsContainer::parseFields(const nlohmann::json& j, const World* w)
{
    for (const auto& field : j) {
        const auto& field_value = field["__value"];
        auto field_type = field["__type"].get<std::string>();
        auto field_name = field["__identifier"].get<std::string>();

        if (field_type.find("Array") != std::string::npos) {
            if (field_value.type() == nlohmann::json::value_t::array) {
                parseArrayField(field_value, field_type, field_name, w);
            }
            else {
                ldtk_error("While parsing Fields, expected array value, got "
                           + std::string(field_value.type_name()));
            }
        }
        else {
            parseValueField(field_value, field_type, field_name, w);
        }
    }
}

void FieldsContainer::parseArrayField(
    const nlohmann::json& field,
    const std::string& type,
    const std::string& name,
    const World* w
)
{
    if (type == "Array<Int>") {
        std::vector<Field<int>> values;
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null) : values.emplace_back(v.get<int>());
        }
        addArrayField(name, values);
    }
    else if (type == "Array<Float>") {
        std::vector<Field<float>> values;
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null) : values.emplace_back(v.get<float>());
        }
        addArrayField(name, values);
    }
    else if (type == "Array<Bool>") {
        std::vector<Field<bool>> values;
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null) : values.emplace_back(v.get<bool>());
        }
        addArrayField(name, values);
    }
    else if (type == "Array<String>" || type == "Array<Multilines>") {
        std::vector<Field<std::string>> values;
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null) : values.emplace_back(v.get<std::string>());
        }
        addArrayField(name, values);
    }
    else if (type == "Array<Color>") {
        std::vector<Field<Color>> values;
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null) : values.emplace_back(Color(v.get<std::string>()));
        }
        addArrayField(name, values);
    }
    else if (type == "Array<Point>") {
        std::vector<Field<IntPoint>> values;
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null)
                        : values.emplace_back(IntPoint(v["cx"].get<int>(), v["cy"].get<int>()));
        }
        addArrayField(name, values);
    }
    else if (type.find("LocalEnum") != std::string::npos) {
        // Extract EnumName from "Array<LocalEnum.EnumName>"
        auto enum_type = type.substr(type.find('.') + 1, type.size() - type.find('.') - 2);
        std::vector<Field<EnumValue>> values;
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null)
                        : values.emplace_back(w->getEnum(enum_type)[v.get<std::string>()]);
        }
        addArrayField(name, values);
    }
    else if (type == "Array<FilePath>") {
        std::vector<Field<FilePath>> values;
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null) : values.emplace_back(v.get<std::string>());
        }
        addArrayField(name, values);
    }
    else if (type == "Array<Tile>") {
        std::vector<Field<TileRect>> values;
        for (const auto& v : field) {
            const auto& tileset = w->getTileset(v["tilesetUid"].get<int>());
            const auto bounds_x = v["x"].get<int>();
            const auto bounds_y = v["y"].get<int>();
            const auto bounds_w = v["w"].get<int>();
            const auto bounds_h = v["h"].get<int>();
            auto bounds = IntRect{bounds_x, bounds_y, bounds_w, bounds_h};
            v.is_null() ? values.emplace_back(null) : values.emplace_back(TileRect{tileset, bounds});
        }
        addArrayField(name, values);
    }
    else if (type == "Array<EntityRef>") {
        std::vector<Field<EntityRef>> values;
        values.reserve(field.size());
        for (const auto& v : field) {
            v.is_null() ? values.emplace_back(null)
                        : values.emplace_back(EntityRef{
                            IID(v["entityIid"].get<std::string>()),
                            IID(v["layerIid"].get<std::string>()),
                            IID(v["levelIid"].get<std::string>()),
                            IID(v["worldIid"].get<std::string>())
                        });
        }
        auto& this_field = addArrayField(name, values);
        for (auto& ent_ref : this_field) {
            temporary_entity_refs_array.emplace_back(&ent_ref.value());
        }
    }
}

void FieldsContainer::parseValueField(
    const nlohmann::json& field,
    const std::string& type,
    const std::string& name,
    const World* w
)
{
    if (type == "Int") {
        field.is_null() ? addField<int>(name, null) : addField<int>(name, field);
    }
    else if (type == "Float") {
        field.is_null() ? addField<float>(name, null) : addField<float>(name, field);
    }
    else if (type == "Bool") {
        field.is_null() ? addField<bool>(name, null) : addField<bool>(name, field);
    }
    else if (type == "String" || type == "Multilines") {
        field.is_null() ? addField<std::string>(name, null) : addField<std::string>(name, field);
    }
    else if (type == "Color") {
        field.is_null() ? addField<Color>(name, null)
                        : addField<Color>(name, Color(field.get<std::string>()));
    }
    else if (type == "Point") {
        field.is_null() ? addField<IntPoint>(name, null)
                        : addField<IntPoint>(name, {field["cx"].get<int>(), field["cy"].get<int>()});
    }
    else if (type.find("LocalEnum") != std::string::npos) {
        // Extract EnumName from "LocalEnum.EnumName"
        auto enum_type = type.substr(type.find('.') + 1, type.size());
        field.is_null() ? addField<EnumValue>(name, null)
                        : addField<EnumValue>(name, w->getEnum(enum_type)[field]);
    }
    else if (type == "FilePath") {
        field.is_null() ? addField<FilePath>(name, null) : addField<FilePath>(name, field.get<std::string>());
    }
    else if (type == "Tile") {
        const auto& tileset = w->getTileset(field["tilesetUid"].get<int>());
        const auto bounds_x = field["x"].get<int>();
        const auto bounds_y = field["y"].get<int>();
        const auto bounds_w = field["w"].get<int>();
        const auto bounds_h = field["h"].get<int>();
        auto bounds = IntRect{bounds_x, bounds_y, bounds_w, bounds_h};
        field.is_null() ? addField<TileRect>(name, null) : addField<TileRect>(name, {tileset, bounds});
    }
    else if (type == "EntityRef") {
        if (field.is_null()) {
            addField<EntityRef>(name, null);
        }
        else {
            auto& this_field = addField<EntityRef>(
                name,
                {IID(field["entityIid"].get<std::string>()),
                 IID(field["layerIid"].get<std::string>()),
                 IID(field["levelIid"].get<std::string>()),
                 IID(field["worldIid"].get<std::string>())}
            );
            temporary_entity_refs_array.emplace_back(&this_field.value());
        }
    }
}
