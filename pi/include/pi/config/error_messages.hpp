#pragma once
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/exceptions.h>

#include <string_view>
#include <string>

namespace YAML::ErrorMsg {

const char* const not_a_sequence = "expecting a sequence";
const char* const not_int_sequence = "expecting an integer sequence";

const char* const not_a_map = "expecting a map";
const char* const not_a_string = "expecting a string";

inline constexpr std::string_view expecting_size(int expected_size)
{
    using namespace std::string_literals;
    return "expecting sequence of size "s + std::to_string(expected_size);
}

inline void error(const YAML::Node& node, std::string_view message)
{
    const YAML::Exception err{ node.Mark(), std::string{ message } };
    std::printf("%s\n", err.what());
}
}
