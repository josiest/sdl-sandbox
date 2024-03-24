#pragma once
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>

inline namespace pi {

inline constexpr auto begin(const SDL_Point& p) { return &p.x; }
inline constexpr auto begin(SDL_Point& p) { return &p.x; }
inline constexpr auto end(const SDL_Point& p) { return (&p.y)+1; }
inline constexpr auto end(SDL_Point& p) { return (&p.y)+1; }

inline constexpr auto begin(const SDL_FPoint& p) { return &p.x; }
inline constexpr auto begin(SDL_FPoint& p) { return &p.x; }
inline constexpr auto end(const SDL_FPoint& p) { return (&p.y)+1; }
inline constexpr auto end(SDL_FPoint& p) { return (&p.y)+1; }

inline constexpr auto begin(const SDL_Rect& rect) { return &rect.x; }
inline constexpr auto begin(SDL_Rect& rect) { return &rect.x; }
inline constexpr auto end(const SDL_Rect& rect) { return (&rect.h)+1; }
inline constexpr auto end(SDL_Rect& rect) { return (&rect.h)+1; }

inline constexpr auto begin(const SDL_FRect& rect) { return &rect.x; }
inline constexpr auto begin(SDL_FRect& rect) { return &rect.x; }
inline constexpr auto end(const SDL_FRect& rect) { return (&rect.h)+1; }
inline constexpr auto end(SDL_FRect& rect) { return (&rect.h)+1; }

inline constexpr auto begin(const SDL_Color& color) { return &color.r; }
inline constexpr auto begin(SDL_Color& color) { return &color.r; }
inline constexpr auto end(const SDL_Color& color) { return (&color.a)+1; }
inline constexpr auto end(SDL_Color& color) { return (&color.a)+1; }
}
