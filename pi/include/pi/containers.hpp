#pragma once
#include <array>
#include <utility>
#include <cstddef>

inline namespace pi {

template<typename K, typename V, std::size_t N>
using lookup_table = std::array<std::pair<K, V>, N>;

template<typename Container, typename Value>
constexpr bool can_push_back = requires(Container& c, Value v)
{
    c.push_back(v);
};
}
