// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/Utils.hpp"

auto ldtk::getLayerTypeFromString(const std::string& type_name) -> ldtk::LayerType
{
    auto ret = LayerType::Tiles;
    if (type_name == "IntGrid") {
        ret = LayerType::IntGrid;
    }
    else if (type_name == "Entities") {
        ret = LayerType::Entities;
    }
    else if (type_name == "Tiles") {
        ret = LayerType::Tiles;
    }
    else if (type_name == "AutoLayer") {
        ret = LayerType::AutoLayer;
    }
    return ret;
}

auto ldtk::getWorldLayoutFromString(const std::string& layout_name) -> ldtk::WorldLayout
{
    auto ret = WorldLayout::Free;
    if (layout_name == "Free") {
        ret = WorldLayout::Free;
    }
    else if (layout_name == "GridVania") {
        ret = WorldLayout::GridVania;
    }
    else if (layout_name == "LinearHorizontal") {
        ret = WorldLayout::LinearHorizontal;
    }
    else if (layout_name == "LinearVertical") {
        ret = WorldLayout::LinearVertical;
    }
    return ret;
}

auto ldtk::getDirFromString(const std::string& dir_name) -> Dir
{
    Dir direction = Dir::None;
    if (dir_name == "n") {
        direction = Dir::North;
    }
    else if (dir_name == "ne") {
        direction = Dir::NorthEast;
    }
    else if (dir_name == "e") {
        direction = Dir::East;
    }
    else if (dir_name == "se") {
        direction = Dir::SouthEast;
    }
    else if (dir_name == "s") {
        direction = Dir::South;
    }
    else if (dir_name == "sw") {
        direction = Dir::SouthWest;
    }
    else if (dir_name == "w") {
        direction = Dir::West;
    }
    else if (dir_name == "nw") {
        direction = Dir::NorthWest;
    }
    else if (dir_name == "o") {
        direction = Dir::Overlap;
    }
    else if (dir_name == ">") {
        direction = Dir::Over;
    }
    else if (dir_name == "<") {
        direction = Dir::Under;
    }

    return direction;
}

auto ldtk::getFieldTypeFromString(const std::string& fieldtype_name) -> ldtk::FieldType
{
    auto ret = FieldType::Int;
    if (fieldtype_name.find("Array") != std::string::npos) {
        if (fieldtype_name == "Array<Int>") {
            ret = FieldType::ArrayInt;
        }
        else if (fieldtype_name == "Array<Float>") {
            ret = FieldType::ArrayFloat;
        }
        else if (fieldtype_name == "Array<Bool>") {
            ret = FieldType::ArrayBool;
        }
        else if (fieldtype_name == "Array<String>" || fieldtype_name == "Array<Multilines>") {
            ret = FieldType::ArrayString;
        }
        else if (fieldtype_name == "Array<Color>") {
            ret = FieldType::ArrayColor;
        }
        else if (fieldtype_name == "Array<Point>") {
            ret = FieldType::ArrayPoint;
        }
        else if (fieldtype_name.find("LocalEnum") != std::string::npos) {
            ret = FieldType::ArrayEnum;
        }
        else if (fieldtype_name == "Array<FilePath>") {
            ret = FieldType::ArrayFilePath;
        }
        else if (fieldtype_name == "Array<EntityRef>") {
            ret = FieldType::ArrayEntityRef;
        }
    }
    else if (fieldtype_name == "Int") {
        ret = FieldType::Int;
    }
    else if (fieldtype_name == "Float") {
        ret = FieldType::Float;
    }
    else if (fieldtype_name == "Bool") {
        ret = FieldType::Bool;
    }
    else if (fieldtype_name == "String" || fieldtype_name == "Multilines") {
        ret = FieldType::String;
    }
    else if (fieldtype_name == "Color") {
        ret = FieldType::Color;
    }
    else if (fieldtype_name == "Point") {
        ret = FieldType::Point;
    }
    else if (fieldtype_name.find("LocalEnum") != std::string::npos) {
        ret = FieldType::Enum;
    }
    else if (fieldtype_name == "FilePath") {
        ret = FieldType::FilePath;
    }
    else if (fieldtype_name == "EntityRef") {
        ret = FieldType::EntityRef;
    }
    return ret;
}

void ldtk::print_error(const std::string& fn, const std::string& msg)
{
    std::cerr << "LDtkLoader Error (in " << fn << ") : " << msg << std::endl;
}

void ldtk::print_json_error(const std::string& msg)
{
    std::cerr << "LDtkLoader Json Error : " << msg << std::endl;
}
