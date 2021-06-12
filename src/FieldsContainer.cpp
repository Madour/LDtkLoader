// Created by Modar Nasser on 11/03/2021.

#include "LDtkLoader/FieldsContainer.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

void FieldsContainer::parseFields(const nlohmann::json& j, const World* w) {
    for (const auto& field : j) {
        auto field_type = field["__type"].get<std::string>();
        auto field_name = field["__identifier"].get<std::string>();
        auto field_value = field["__value"];

        // array fields
        if (field_type.find("Array") != std::string::npos) {
            if (field_type == "Array<Int>") {
                std::vector<Field<int>> values;
                for (const auto& v : field_value) {
                    if (v.is_null())
                        values.emplace_back(null);
                    else
                        values.emplace_back(v.get<int>());
                }
                addArrayField(field_name, values);
            }
            else if (field_type == "Array<Float>") {
                std::vector<Field<float>> values;
                for (const auto& v : field_value) {
                    if (v.is_null())
                        values.emplace_back(null);
                    else
                        values.emplace_back(v.get<float>());
                }
                addArrayField(field_name, values);
            }
            else if (field_type == "Array<Bool>") {
                std::vector<Field<bool>> values;
                for (const auto& v : field_value) {
                    if (v.is_null())
                        values.emplace_back(null);
                    else
                        values.emplace_back(v.get<bool>());
                }
                addArrayField(field_name, values);
            }
            else if (field_type == "Array<String>") {
                std::vector<Field<std::string>> values;
                for (const auto& v : field_value) {
                    if (v.is_null())
                        values.emplace_back(null);
                    else
                        values.emplace_back(v.get<std::string>());
                }
                addArrayField(field_name, values);
            }
            else if (field_type == "Array<Color>") {
                std::vector<Field<Color>> values;
                for (const auto& v : field_value) {
                    if (v.is_null())
                        values.emplace_back(null);
                    else
                        values.emplace_back(Color(v.get<std::string>()));
                }
                addArrayField(field_name, values);
            }
            else if (field_type == "Array<Point>") {
                std::vector<Field<IntPoint>> values;
                for (const auto& v : field_value) {
                    if (v.is_null())
                        values.emplace_back(null);
                    else
                        values.emplace_back(IntPoint(v["cx"].get<int>(), v["cy"].get<int>()));
                }
                addArrayField(field_name, values);
            }
            else if (field_type.find("LocalEnum") != std::string::npos) {
                auto enum_type = field_type.substr(field_type.find('.')+1, field_type.size()-field_type.find('.')-2);
                std::vector<Field<EnumValue>> values;
                for (const auto& v : field_value) {
                    if (v.is_null())
                        values.emplace_back(null);
                    else
                        values.emplace_back(w->getEnum(enum_type)[v.get<std::string>()]);
                }
                addArrayField(field_name, values);
            }
            else if (field_type == "Array<FilePath>") {
                std::vector<Field<FilePath>> values;
                for (const auto& v : field_value) {
                    if (v.is_null())
                        values.emplace_back(null);
                    else
                        values.emplace_back(v.get<std::string>());
                }
                addArrayField(field_name, values);
            }
        }
            // simple fields
        else if (field_type == "Int") {
            if (field_value.is_null())
                addField<int>(field_name, null);
            else
                addField<int>(field_name, field_value);
        }
        else if (field_type == "Float") {
            if (field_value.is_null())
                addField<float>(field_name, null);
            else
                addField<float>(field_name, field_value);
        }
        else if (field_type == "Bool") {
            if (field_value.is_null())
                addField<bool>(field_name, null);
            else
                addField<bool>(field_name, field_value);
        }
        else if (field_type == "String") {
            if (field_value.is_null())
                addField<std::string>(field_name, null);
            else
                addField<std::string>(field_name, field_value);
        }
        else if (field_type == "Color") {
            if (field_value.is_null())
                addField<Color>(field_name, null);
            else
                addField<Color>(field_name, Color(field_value.get<std::string>()));
        }
        else if (field_type == "Point") {
            if (field_value.is_null())
                addField<IntPoint>(field_name, null);
            else
                addField<IntPoint>(field_name, {field_value["cx"].get<int>(), field_value["cy"].get<int>()});
        }
        else if (field_type.find("LocalEnum") != std::string::npos) {
            auto enum_type = field_type.substr(field_type.find('.')+1, field_type.size());
            if (field_value.is_null())
                addField<EnumValue>(field_name, null);
            else
                addField<EnumValue>(field_name, w->getEnum(enum_type)[field_value]);
        }
        else if (field_type == "FilePath") {
            if (field_value.is_null())
                addField<FilePath>(field_name, null);
            else
                addField<FilePath>(field_name, field_value.get<std::string>());
        }
    }
}
