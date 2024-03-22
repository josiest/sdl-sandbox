#include "game/world_system.hpp"

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