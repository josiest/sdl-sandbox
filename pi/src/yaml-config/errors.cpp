#include "pi/yaml-config/errors.hpp"
#include <yaml-cpp/exceptions.h>
#include <format>

std::string YAML::ErrorMsg::expecting_size(int expected_size)
{
    return std::format("expecting sequence of size {}", expected_size);
}

std::string YAML::ErrorMsg::flag_not_found(std::string_view name)
{
    return std::format("couldn't find flag value for {}", name);
}

void YAML::ErrorMsg::error(const YAML::Node& node, std::string_view message)
{
    const YAML::Exception err{ node.Mark(), std::string{ message } };
    std::printf("%s\n", err.what());
}
