// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <cinttypes>
#include <string>

template <typename T>
struct Point {
    T x;
    T y;
};

struct Color {
    Color();
    explicit Color(const std::string& hex);
    Color(unsigned int red, unsigned int green, unsigned int bblue);

    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;

};