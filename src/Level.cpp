// Created by Modar Nasser on 12/11/2020.

#include "Level.hpp"

using namespace ldtk;

Level::Level(const nlohmann::json& j) :
name(j["identitifer"].get<std::string>()),
uid(j["uid"].get<unsigned int>()),
size({j["pxWid"].get<unsigned int>(), j["pxHei"].get<unsigned int>()})
{

}
