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
    SDL_Point bounds{ 800, 600 };
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
    SDL_GetRendererOutputSize(renderer, &bounds.x, &bounds.y);
}