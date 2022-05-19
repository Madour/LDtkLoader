// Created by Modar Nasser on 22/02/2022.

#pragma once

#include <string>
#include <vector>

#include "LDtkLoader/thirdparty/json_fwd.hpp"

namespace ldtk {
    class TagsContainer {
    public:
        auto hasTag(const std::string& tag) const -> bool;
        auto getTags() const -> const std::vector<std::string>&;

    protected:
        explicit TagsContainer(const nlohmann::json& j);

    private:
        const std::vector<std::string> m_tags;
    };

}
