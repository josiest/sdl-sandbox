#pragma once
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/exceptions.h>

#include <string_view>
#include <string>
#include <format>

namespace YAML::ErrorMsg {

constexpr char invalid_node[] = "yaml node is invalid";

constexpr char not_an_integer[] = "expecting an integer";
constexpr char not_a_string[] = "expecting a string";

constexpr char not_a_sequence[] = "expecting a sequence";
constexpr char not_int_sequence[] = "expecting an integer sequence";

constexpr char not_a_map[] = "expecting a map";

constexpr char too_many_flags[] =
    "encountered more than 32 flag names, but flags have only 32 bits";

inline std::string expecting_size(int expected_size)
{
    return std::format("expecting sequence of size {}", expected_size);
}

inline std::string flag_not_found(std::string_view name)
{
    return std::format("couldn't find flag value for {}", name);
}

inline void error(const YAML::Node& node, std::string_view message)
{
    const YAML::Exception err{ node.Mark(), std::string{ message } };
    std::printf("%s\n", err.what());
}
}
