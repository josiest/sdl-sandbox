#pragma once
#include <entt/meta/factory.hpp>

inline namespace pi {

template<typename T>
[[maybe_unused]] auto reflect()
{
    return entt::meta<T>();
}

template<typename T>
concept reflectable = requires {
    { reflect<T>() } -> std::same_as<entt::meta_factory<T>>;
};
}
