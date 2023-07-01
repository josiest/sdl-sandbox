#pragma once
#include "pi/config/errors.hpp"

#include <cstdio>
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
