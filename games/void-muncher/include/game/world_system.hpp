#pragma once
#include <random>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <entt/entity/registry.hpp>
#include "pi/systems/system_graph.hpp"

inline namespace munch {

struct world_system {
    static world_system* load(pi::system_graph& systems);

    world_system();
    void init(SDL_Renderer* renderer);

    entt::registry entities;
    std::mt19937 rng;
    SDL_FRect bounds{ 0.f, 0.f, 800.f, 600.f };
};
}
