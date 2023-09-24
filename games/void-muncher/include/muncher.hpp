#pragma once
#include <cmath>
#include <algorithm>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "pi/algorithm/primitives.hpp"
#include "pi/events/event_sink.hpp"

#include "pi/meta/reflect.hpp"
#include "pi/meta/color.hpp"
#include "pi/config/color.hpp"

#include "input/keyboard_axis.hpp"

inline namespace munch {
struct muncher_data {
    SDL_Color color{ 0x0, 0x0, 0x0, 0xff };
    float starting_size = 20.f;
    float speed = 10.f;
};
}

inline namespace pi {
template<>
auto reflect<muncher_data>()
{
    using namespace entt::literals;
    YAML::convert<SDL_Color>::reflect();

    return entt::meta<munch::muncher_data>()
        .type("muncher-settings"_hs)
        .data<&muncher_data::color>("color"_hs)
        .data<&muncher_data::starting_size>("starting-size"_hs)
        .data<&muncher_data::speed>("speed"_hs);
}
}

inline namespace munch {
class muncher {
public:
    muncher()
        : config{},
          bbox{ 0.f, 0.f, config.starting_size, config.starting_size }
    {
    }
    explicit muncher(const muncher_data& in_config)
        : config{ in_config },
          bbox{ 0.f, 0.f, config.starting_size, config.starting_size }
    {
    }

    inline void connect_to(pi::keyboard_axis& axis)
    {
        axis.on_tick().connect<&muncher::move>(*this);
    }
    void move(const pi::axis2d& axis, float delta_time)
    {
        bbox.x += delta_time * config.speed * static_cast<float>(axis.first);
        bbox.y += delta_time * config.speed * static_cast<float>(axis.second);
    }
    void reset(SDL_Renderer* renderer)
    {
        int width = 0; int height = 0;
        SDL_GetRendererOutputSize(renderer, &width, &height);
        bbox.x = static_cast<float>(width)/2.f;
        bbox.y = static_cast<float>(height)/2.f;
    }
    [[nodiscard]] inline SDL_Rect rounded_bbox() const
    {
        namespace ranges = std::ranges;
        SDL_Rect rounded;
        auto round_to_int = [](float v) { return static_cast<int>(std::round(v)); };
        ranges::transform(bbox, ranges::begin(rounded), round_to_int);
        return rounded;
    }
    muncher_data config;
    SDL_FRect bbox;
};

inline void draw(SDL_Renderer* renderer, const muncher& player)
{
    SDL_SetRenderDrawColor(renderer, player.config.color.r, player.config.color.g,
                                     player.config.color.b, player.config.color.a);

    const SDL_Rect bbox = player.rounded_bbox();
    SDL_RenderFillRect(renderer, &bbox);
}
}