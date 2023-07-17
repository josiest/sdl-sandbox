#pragma once
#include <entt/meta/factory.hpp>

inline namespace pi {

template<typename T>
auto reflect()
{
    return entt::meta<T>();
}
}
