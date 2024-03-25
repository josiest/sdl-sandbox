#pragma once
#include <concepts>
#include <ranges>
#include <algorithm>

#include <functional>
#include <numbers>
#include <utility>

#include <cstdint>
#include <cmath>

#include <SDL2/SDL_render.h>
#include "spiral_params.hpp"

inline namespace fib {
/** Truncate-lerp between two integers */
template<std::integral Integer, std::floating_point Real>
constexpr Integer lerp(Integer a, Integer b, Real t)
{
    return static_cast<Integer>(std::lerp(static_cast<Real>(a), static_cast<Real>(b), t));
}
/** Lerp between two colors */
template<std::floating_point Real>
constexpr SDL_Color lerp(const SDL_Color& a, const SDL_Color& b, Real t)
{
    return SDL_Color{ fib::lerp(a.r, b.r, t), fib::lerp(a.g, b.g, t),
                      fib::lerp(a.b, b.b, t), fib::lerp(a.a, b.a, t) };
}
/** Divide an integer by a floating point number and round to nearest */
template<std::integral Integer, std::floating_point Real>
constexpr Integer rounded_divide(Integer numerator, Real denominator)
{
    const Real product = static_cast<Real>(numerator)/denominator;
    return static_cast<Integer>(std::round(product));
}
/** Divide two integers as a floating point type */
template<std::floating_point Real, std::integral IntegerA, std::integral IntegerB>
constexpr Real divide(IntegerA numerator, IntegerB denominator)
{
    return static_cast<Real>(numerator)/static_cast<Real>(denominator);
}

/** Transform a guide rect into the next subframe of the fibonacci spiral
 *
 * \param guide changed into the next fibonacci spiral frame
 * \param k     used to determine which edge to transform
 *
 * \return the current value of the guide rect unchanged
 */
constexpr SDL_Rect next_subframe(SDL_Rect& guide, std::uint32_t k)
{
    using namespace std::numbers;
    const SDL_Rect rect = guide;
    const SDL_Point frame{ rounded_divide(guide.w, phi_v<float>),
                           rounded_divide(guide.h, phi_v<float>) };
    switch (k % 4) {
    case 0:
        // slide the left edge inward by the golden ratio
        guide.x += frame.x; guide.w -= frame.x;
        break;
    case 1:
        // slide the top edge down by the golden ratio
        guide.y += frame.y; guide.h -= frame.y;
        break;
    case 2:
        // slide the right edge inward by the golden ratio
        guide.w -= frame.x;
        break;
    case 3:
        // slide the bottom edge upward by the golden ratio
        guide.h -= frame.y;
        break;
    default: break;
    }
    return rect;
}

/** Draw a colored rect to a renderer */
inline void fill_rect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

/** Generate the fibonacci spiral sequence
 *
 * \param guide The frame to generate te spiral in
 * \return a function that generates the fibonacci spiral sequence
 *
 * As a side effect, the guide will be changed with every call
 * of the generator function
 */
inline auto generate_sequence(const spiral_data& spiral, SDL_Rect& guide)
{
    return [&](std::uint32_t k) {
        const auto t = divide<float>(k, spiral.num_frames);
        const auto color = fib::lerp(spiral.initial_color, spiral.final_color, t);
        return std::make_pair(next_subframe(guide, k), color);
    };
}
/** Draw the fibonacci spiral to the renderer */
inline void draw_spiral(SDL_Renderer* renderer, const spiral_data& spiral)
{
    namespace ranges = std::ranges;
    namespace views = std::ranges::views;

    // the pattern will be drawn to the entire screen
    SDL_Rect render_frame{ 0, 0, 0, 0 };
    SDL_GetRendererOutputSize(renderer, &render_frame.w, &render_frame.h);

    // render each subframe with the next color on the gradient
    auto colored_rect_sequence = views::iota(0u, spiral.num_frames)
                               | views::transform(generate_sequence(spiral, render_frame));
    for (const auto& [rect, color] : colored_rect_sequence) {
        fill_rect(renderer, rect, color);
    }
}
}