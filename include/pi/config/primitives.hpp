#pragma once
#include "pi/config/errors.hpp"

#include <algorithm>
#include <bitset>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>

#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/convert.h>

template<>
struct YAML::convert<SDL_Point> {
    static YAML::Node encode(const SDL_Point& point)
    {
        YAML::Node node;
        node.push_back(point.x);
        node.push_back(point.y);
        return node;
    }

    static bool decode(const YAML::Node& node, SDL_Point& point)
    {
        namespace msg = YAML::ErrorMsg;

        if (not node) { return false; }
        if (not node.IsSequence()) {
            msg::error(node, msg::not_a_sequence);
            return false;
        }
        if (not node.size() == 2) {
            msg::error(node, msg::expecting_size(2));
            return false;
        }
        using as_int = YAML::convert<int>;
        const bool has_x = as_int::decode(node[0], point.x);
        const bool has_y = as_int::decode(node[1], point.y);
        if (not has_x or not has_y) {
            msg::error(node, msg::not_int_sequence);
            return false;
        }
        return true;
    }
};

template<>
struct YAML::convert<SDL_Color> {
    static YAML::Node encode(const SDL_Color& color)
    {
        YAML::Node node;
        node.push_back(color.r);
        node.push_back(color.g);
        node.push_back(color.b);
        node.push_back(color.a);
        return node;
    }

    static bool decode(const YAML::Node& node, SDL_Color& point)
    {
        namespace msg = YAML::ErrorMsg;
        namespace ranges = std::ranges;
        namespace views = std::views;

        if (not node) { return false; }
        if (not node.IsSequence()) {
            msg::error(node, msg::not_a_sequence);
            return false;
        }
        constexpr std::uint32_t num_values = 4;
        if (not node.size() == num_values) {
            msg::error(node, msg::expecting_size(num_values));
            return false;
        }
        using as_int = YAML::convert<int>;
        std::array<int, num_values> values = { 0 };
        std::bitset<num_values> has_value;

        ranges::for_each(views::iota(0u, num_values), [&](auto i) {
            has_value.set(i, as_int::decode(node[i], values[i]));
        });
        constexpr std::string_view message =
            "all color values must be integers in [0-255]";
        if (not has_value.all()) {
            msg::error(node, message); return false;
        }
        if (ranges::any_of(values, [](auto v) { return v < 0 || v > 255; })) {
            msg::error(node, message); return false;
        }
        point.r = static_cast<std::uint8_t>(values[0]);
        point.g = static_cast<std::uint8_t>(values[1]);
        point.b = static_cast<std::uint8_t>(values[2]);
        point.a = static_cast<std::uint8_t>(values[3]);
        return true;
    }
};
