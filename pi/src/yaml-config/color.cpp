#include "pi/yaml-config/color.hpp"

std::string pi::write_hex_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    static char buf[8];
    std::sprintf(buf, "0x%2x%2x%2x", r, g, b);
    return std::string{ buf };
}

std::string pi::write_hex_rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
    static char buf[10];
    std::sprintf(buf, "0x%2x%2x%2x%2x", r, g, b, a);
    return std::string{ buf };
}
