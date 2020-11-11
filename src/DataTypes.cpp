// Created by Modar Nasser on 11/11/2020.

#include "LDtkLoader/DataTypes.hpp"

Color::Color() = default;

Color::Color(unsigned int red, unsigned int green, unsigned int blue) :
        r(red&0xffu), g(green&0xffu), b(blue&0xffu)
{}

Color::Color(const std::string& hex) :
        Color(std::stoul(hex.substr(1, 2), nullptr, 16),
              std::stoul(hex.substr(3, 2), nullptr, 16),
              std::stoul(hex.substr(5, 2), nullptr, 16))
{}
