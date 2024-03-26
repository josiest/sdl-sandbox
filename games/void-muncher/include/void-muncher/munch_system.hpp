#pragma once
#include <cstdint>

#include <entt/entity/fwd.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/factory.hpp>

#include "pi/reflection/reflect.hpp"

struct SDL_Renderer;

inline namespace munch {
struct score_params {
    std::uint32_t score_per_munchable = 10;
};

struct score_widget {
    float width = 100.f;
    float outer_padding = 50.f;
    float inner_padding = 20.f;
    std::string font_name = "ProggyClean";
};
}

inline namespace pi {
template<>
inline auto reflect<munch::score_params>()
{
    using namespace entt::literals;
    return entt::meta<munch::score_params>()
        .type("scoring-system"_hs)
        .data<&munch::score_params::score_per_munchable>("score-per-munchable"_hs);
}

template<>
inline auto reflect<munch::score_widget>()
{
    using namespace entt::literals;
    return entt::meta<munch::score_widget>()
        .type("score-widget"_hs)
        .data<&munch::score_widget::width>("width"_hs)
        .data<&munch::score_widget::inner_padding>("inner-padding"_hs)
        .data<&munch::score_widget::outer_padding>("outer-padding"_hs)
        .data<&munch::score_widget::font_name>("font-name"_hs);
}
}

inline namespace munch {
struct munch_system {
    void munch_or_be_munched(entt::registry & entities, entt::entity player);
    void draw_score(SDL_Renderer * renderer);

    score_params config;
    score_widget widget;
    std::uint32_t score = 0;
};
}