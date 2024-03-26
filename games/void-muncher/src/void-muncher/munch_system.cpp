#include "void-muncher/munch_system.hpp"
#include "basic-components/basic_components.hpp"

#include <format>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <entt/entity/registry.hpp>

#include "imgui/imgui.h"


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
        }
        else {
            entities.destroy(player);
        }
    }
}

void munch_system::draw_score(SDL_Renderer * renderer)
{
    int screen_width, screen_height;
    SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);

    const std::string score_text = std::format("{0}", score);
    const ImVec2 text_size = ImGui::CalcTextSize(score_text.c_str());
    const float widget_height = text_size.y + static_cast<float>(2*widget.inner_padding);

    const SDL_FRect score_bounds{
        static_cast<float>(screen_width) - widget.outer_padding - widget.width,
        static_cast<float>(screen_height) - widget.outer_padding - widget_height,
        widget.width, widget_height
    };
    ImGui::SetNextWindowPos(ImVec2{ score_bounds.x, score_bounds.y });
    ImGui::SetNextWindowSize(ImVec2{ score_bounds.w, score_bounds.h });

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove
                                  | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("score-window", nullptr, window_flags);
    ImGui::SetCursorPos(ImVec2{ (widget.width - text_size.x) * 0.5f, widget.inner_padding });
    ImGui::Text(score_text.c_str());
    ImGui::End();
}