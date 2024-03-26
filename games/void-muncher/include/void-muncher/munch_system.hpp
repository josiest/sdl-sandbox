#pragma once
#include <cstdint>

#include <entt/entity/fwd.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/factory.hpp>

#include "pi/reflection/reflect.hpp"

inline namespace munch {
struct score_params {
    std::uint32_t score_per_munchable = 10;
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
}

inline namespace munch {
struct munch_system {
    void munch_or_be_munched(entt::registry & entities, entt::entity player);

    score_params config;
    std::uint32_t score = 0;
};
}