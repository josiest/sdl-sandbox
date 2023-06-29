#pragma once
#include <array>
#include <utility>
#include <cstddef>

inline namespace pi {

template<typename Container, typename Value>
constexpr bool can_push_back = requires(Container& c, Value v)
{
    c.push_back(v);
};
}
