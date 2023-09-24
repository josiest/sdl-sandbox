#pragma once

#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>

#include <string>
#include <string_view>

#include "pi/config/meta.hpp"

inline namespace fib {
struct gameplay_settings {
    std::string spiral = "peach";
};
}

inline namespace pi {
template<>
auto reflect<fib::gameplay_settings>()
{
    using namespace entt::literals;
    return entt::meta<fib::gameplay_settings>()
        .type("gameplay-settings"_hs)
        .data<&fib::gameplay_settings::spiral>("spiral"_hs);
}
}
