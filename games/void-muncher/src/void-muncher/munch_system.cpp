#include "void-muncher/munch_system.hpp"
#include "basic-components/basic_components.hpp"

#include <SDL2/SDL_rect.h>
#include <entt/entity/registry.hpp>

void munch_system::munch_or_be_munched(entt::registry & entities, entt::entity player)
{
    if (not entities.valid(player)) { return; }
    const SDL_FRect player_bbox = entities.get<component::bbox>(player);

    auto munchables = entities.view<component::bbox>();
    for (const auto & [munchable, munchable_bbox] : munchables.each()) {

        const SDL_FRect munchable_b_rect = munchable_bbox;
        if (munchable == player) { continue; }
        if (not SDL_HasIntersectionF(&munchable_b_rect, &player_bbox)) { continue; }
        if (player_bbox.w > munchable_bbox.size) {
            entities.destroy(munchable);
            score += config.score_per_munchable;
            std::printf("current score: %d\n", score);
        }
        else {
            entities.destroy(player);
        }
    }
}