#pragma once
#include <yaml-cpp/node/node.h>

#include <string_view>
#include <string>

namespace YAML::ErrorMsg {

constexpr char invalid_node[] = "yaml node is invalid";

constexpr char not_an_integer[] = "expecting an integer";
constexpr char not_a_string[] = "expecting a string";
constexpr char not_hex_color[] = "expecting a hex color (e.g 0x4287f5ff)";

constexpr char not_a_sequence[] = "expecting a sequence";
constexpr char not_int_sequence[] = "expecting an integer sequence";

constexpr char not_a_map[] = "expecting a map";

constexpr char too_many_flags[] =
    "encountered more than 32 flag names, but flags have only 32 bits";

std::string expecting_size(int expected_size);
std::string flag_not_found(std::string_view name);
void error(const YAML::Node& node, std::string_view message);

}
