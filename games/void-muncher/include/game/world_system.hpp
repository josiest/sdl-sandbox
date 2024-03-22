#pragma once
#include <random>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <entt/entity/registry.hpp>
#include "pi/systems/system_graph.hpp"

inline namespace munch {

struct world_system {
    inline world_system();
    inline static world_system* load(pi::system_graph& systems);
    inline void init(SDL_Renderer* renderer);

    entt::registry entities;
    std::mt19937 rng;
    SDL_FRect bounds{ -400.f, -300.f, 800.f, 600.f };
};
}

munch::world_system::world_system()
    : rng{ std::random_device{}() }
{
}

munch::world_system* munch::world_system::load(pi::system_graph & systems)
{
    return &systems.emplace<world_system>();
}

void munch::world_system::init(SDL_Renderer* renderer)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    const float width =  static_cast<float>(w);
    const float height = static_cast<float>(h);
    bounds = { -width/2.f, -height/2.f, width, height };
}