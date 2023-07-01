#pragma once
#include "pi/config/error_messages.hpp"

#include <type_traits>
#include <concepts>
#include <iterator>
#include <ranges>

#include <algorithm>
#include <numeric>
#include <functional>

#include <utility>
#include <string>
#include <string_view>

#include <yaml-cpp/yaml.h>

namespace pi {

template<typename EnumType, typename Integer>
concept yaml_enum = requires(std::string_view name) {
    requires std::is_enum_v<EnumType>;
    requires std::unsigned_integral<Integer>;

    { YAML::convert<EnumType>::begin() } -> std::forward_iterator;
    { YAML::convert<EnumType>::end() } ->
        std::sentinel_for<decltype(YAML::convert<EnumType>::begin())>;

    { *YAML::convert<EnumType>::begin() } ->
        std::convertible_to<std::pair<Integer, std::string_view>>;

    { YAML::convert<EnumType>::find(name) } ->
        std::same_as<decltype(YAML::convert<EnumType>::begin())>;
};

template<typename EnumType, std::unsigned_integral FlagType>
requires yaml_enum<EnumType, FlagType>
YAML::Node encode_flags(FlagType flags)
{
    YAML::Node names;
    auto encode = [&](const auto& flag_pair) {
        const auto& [flag, name] = flag_pair;
        if ((flags & flag) == flag) {
            names.push_back(std::string{ name });
        }
    };
    using lookup = YAML::convert<EnumType>;
    std::ranges::for_each(lookup::begin(), lookup::end(), encode);
    return names;
}

template<typename EnumType, std::unsigned_integral FlagType>
requires yaml_enum<EnumType, FlagType>
bool read_flag_into(const YAML::Node& node, FlagType& value)
{
    namespace msg = YAML::ErrorMsg;

    if (not node) { return false; }
    if (not node.IsScalar()) {
        msg::error(node, msg::not_a_string);
        return false;
    }
    const auto name = node.as<std::string>();

    using lookup = YAML::convert<EnumType>;
    const auto search = lookup::find(name);
    if (search != lookup::end()) {
        value |= search->first;
        return true;
    }
    msg::error(node, msg::flag_not_found(name));
    return false;
}

template<typename EnumType, std::unsigned_integral FlagType>
requires yaml_enum<EnumType, FlagType>
bool read_flags_into(const YAML::Node& node, FlagType& flags)
{
    namespace msg = YAML::ErrorMsg;

    if (not node) { return false; }
    if (node.IsScalar()) {
        return read_flag_into<EnumType>(node, flags);
    }
    if (not node.IsSequence()) {
        msg::error(node, msg::not_a_sequence);
        return false;
    }
    auto read_into_flags = [&](const YAML::Node& elem) {
        return read_flag_into<EnumType>(elem, flags);
    };
    return std::transform_reduce(node.begin(), node.end(),
                                 true, std::logical_and{}, read_into_flags);
}
}
