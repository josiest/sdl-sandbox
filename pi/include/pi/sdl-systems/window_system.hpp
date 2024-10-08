#pragma once
#include <iterator>
#include <algorithm>

#include <array>

#include <memory>
#include <cstdio>

#include <SDL2/SDL_video.h>
#include <entt/core/type_info.hpp>

#include "pi/sdl-systems/sdl_system.hpp"
#include "pi/sdl-resources/sdl_deleter.hpp"

namespace YAML {
class Node;
}

inline namespace pi {
struct window_params;
class system_graph;

struct window_system {

    template<std::output_iterator<entt::id_type> TypeOutput>
    inline static TypeOutput dependencies(TypeOutput into_dependencies)
    {
        namespace ranges = std::ranges;
        return ranges::copy(std::array{ entt::type_hash<sdl_system>::value() },
                            into_dependencies).out;
    }

    static window_system* load(system_graph& systems);
    static window_system* load(system_graph& systems, const YAML::Node& root);
    [[nodiscard]] SDL_Window* window() const;

    unique_window window_handle;

private:
    static window_system* load(system_graph& systems, const pi::window_params& params);
};
}
