#pragma once
#include <concepts>
#include <ranges>
#include <algorithm>
#include <numeric>

#include <utility>
#include <string>
#include <string_view>

#include <yaml-cpp/yaml.h>

namespace pi {

template<std::unsigned_integral FlagType,
         std::ranges::input_range LookupRange>

requires std::convertible_to<std::ranges::range_value_t<LookupRange>,
                             std::pair<FlagType, std::string_view>>

void write_flags(LookupRange && lookup_table,
                 FlagType flags, YAML::Node& into_names)
{
    for (const auto [flag, name] : lookup_table) {
        if ((flags & flag) == flag) {
            into_names.push_back(std::string{ name });
        }
    }
}

template<std::unsigned_integral FlagType,
         std::ranges::input_range LookupRange>

requires std::convertible_to<std::ranges::range_value_t<LookupRange>,
                             std::pair<FlagType, std::string_view>>

inline constexpr FlagType
read_flag_name(LookupRange && lookup_table,
               const YAML::Node& node, FlagType default_value)
{
    namespace ranges = std::ranges;

    if (not node.IsScalar()) {
        const YAML::Exception error{ node.Mark(), "Scalar Error" };
        std::printf("%s\nExpecting a string value\n", error.what());
        return default_value;
    }
    const auto name = node.as<std::string>();

    using lookup_elem = std::ranges::range_value_t<LookupRange>;
    if (const auto search = ranges::find(lookup_table, name,
                                         &lookup_elem::second)) {
        return search->first;
    }
    const YAML::Exception error{ node.Mark(), "Flag Error" };
    std::printf("%s\nCouldn't find flag value for %s\n",
                error.what(), name.c_str());
    return default_value;
}

template<std::unsigned_integral FlagType,
         std::ranges::input_range LookupRange>

inline constexpr FlagType
read_flag_names(const LookupRange& lookup_table,
                const YAML::Node& sequence, FlagType init)
{
    namespace ranges = std::ranges;
    if (not sequence.IsSequence()) { return init; }

    constexpr FlagType zero = 0;
    auto read_flag = [&](const auto& node) {
        return read_flag_name(lookup_table, node, zero);
    };
    return std::transform_reduce(sequence.begin(), sequence.end(),
                                 init, std::bit_or{}, read_flag);
}
}
