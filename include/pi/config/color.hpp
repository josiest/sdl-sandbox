#pragma once
#include <concepts>

#include <cstdint>
#include <string>
#include <string_view>

#include <regex>
#include <cstdio>

#include <yaml-cpp/node/convert.h>
#include "pi/config/errors.hpp"

inline namespace pi {

namespace patterns {
constexpr std::string_view hexadecimal_rgb = "0x([0-9a-fA-F]{6})";
constexpr std::string_view hexadecimal_rgba = "0x([0-9a-fA-F]{8})";
}

template<std::unsigned_integral Integer>
bool parse_hex_rgb(std::string_view text, Integer& r, Integer& g, Integer& b)
{
    static const std::regex hex_pattern{ patterns::hexadecimal_rgb.data() };
    std::cmatch match;
    if (not std::regex_match(text.data(), match, hex_pattern)) {
        return false;
    }
    std::sscanf(match.str(1).c_str(), "%2x%2x%2x", &r, &g, &b);
    return true;
}

template<std::unsigned_integral Integer>
bool parse_hex_rgba(std::string_view text, Integer& r, Integer& g,
                                           Integer& b, Integer& a)
{
    static const std::regex hex_pattern{ patterns::hexadecimal_rgba.data() };
    std::cmatch match;
    if (not std::regex_match(text.data(), match, hex_pattern)) {
        return false;
    }
    std::sscanf(match.str(1).c_str(), "%2x%2x%2x%2x", &r, &g, &b, &a);
    return true;
}

std::string write_hex_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    static char buf[8];
    std::printf(buf, "0x%2x%2x%2x", r, g, b);
    return std::string{ buf };
}

std::string write_hex_rgba(std::uint8_t r, std::uint8_t g,
                           std::uint8_t b, std::uint8_t a)
{
    static char buf[10];
    std::printf(buf, "0x%2x%2x%2x%2x", r, g, b, a);
    return std::string{ buf };
}
}

template<>
struct YAML::convert<SDL_Color> {
    static YAML::Node encode(const SDL_Color& color)
    {
        using as_string = YAML::convert<std::string>;
        return as_string::encode(write_hex_rgba(color.r, color.g,
                                                color.b, color.a));
    }

    static bool decode(const YAML::Node& node, SDL_Color& color)
    {
        namespace msg = YAML::ErrorMsg;
        if (not node) {
            std::printf("%s\n", msg::invalid_node);
            return false;
        }
        if (not node.IsScalar()) {
            msg::error(node, msg::not_hex_color);
            return false;
        }
        if (parse_hex_rgba(node.Scalar(), color.r, color.g,
                                          color.b, color.a)) {
            return true;
        }
        if (parse_hex_rgb(node.Scalar(), color.r, color.g, color.b)) {
            return true;
        }
        msg::error(node, msg::not_hex_color);
        return false;
    }
};
