// Created by Modar Nasser on 22/02/2022.

#include "LDtkLoader/TagsContainer.hpp"


using namespace ldtk;

TagsContainer::TagsContainer(const nlohmann::json& j) :
m_tags(j.get<std::vector<std::string>>())
{}

auto TagsContainer::hasTag(const std::string& tag) -> bool {
    auto pos = std::find(m_tags.begin(), m_tags.end(), tag);
    return pos != m_tags.end();
}

auto TagsContainer::getTags() -> const std::vector<std::string>& {
    return m_tags;
}
