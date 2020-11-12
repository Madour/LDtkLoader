// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {

    class World;

    class Level {
    public:
        const std::string name;
        const unsigned int uid;
        const UIntPoint size;

    private:
        explicit Level(const nlohmann::json& j);
    };

}

