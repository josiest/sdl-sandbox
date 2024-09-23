#pragma once
#include <iterator>
#include <algorithm>

#include <array>

#include <memory>
#include <cstdio>

#include <SDL2/SDL_render.h>
#include <entt/core/type_info.hpp>

#include "pi/sdl-systems/window_system.hpp"
#include "pi/sdl-resources/sdl_deleter.hpp"

inline namespace pi {

struct renderer_params;
class system_graph;

class renderer_system {
public:
    template<std::output_iterator<entt::id_type> TypeOutput>
    inline static TypeOutput dependencies(TypeOutput into_types)
    {
        namespace ranges = std::ranges;
        return ranges::copy(
                std::array{ entt::type_hash<window_system>::value() },
                into_types).out;
    }

    static renderer_system* load(pi::system_graph& systems);
    static renderer_system* load(pi::system_graph& systems, const YAML::Node& root);
    SDL_Renderer* renderer();

    unique_renderer renderer_handle;
private:
    static renderer_system* load(pi::system_graph& systems, SDL_Window* window,
                                 const pi::renderer_params& params);

};
}
