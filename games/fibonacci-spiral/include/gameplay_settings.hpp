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
gameplay_settings load_settings(std::string_view settings_path);
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

fib::gameplay_settings
fib::load_settings(std::string_view settings_path)
{
    const auto config = pi::load_config(settings_path);
    fib::gameplay_settings settings;
    if (config) {
        if (pi::config::decode(*config, settings)) { return settings; }
    }
    else {
        std::printf("%s\n", config.error().c_str());
    }
    std::printf("unable to load gameplay settings, using defaults\n");
    return settings;
}