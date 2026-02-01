#include "ui/renderer.h"

namespace vita::ui {

Renderer::Renderer(SDL_Renderer *renderer) : renderer_(renderer) {}

void Renderer::Clear(const SDL_Color &color) const {
  SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
  SDL_RenderClear(renderer_);
}

void Renderer::DrawRect(int x, int y, int w, int h, const SDL_Color &color) const {
  SDL_Rect rect{x, y, w, h};
  SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer_, &rect);
}

void Renderer::DrawRectOutline(int x, int y, int w, int h, const SDL_Color &color, int thickness) const {
  SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
  for (int i = 0; i < thickness; ++i) {
    SDL_Rect rect{x + i, y + i, w - (i * 2), h - (i * 2)};
    SDL_RenderDrawRect(renderer_, &rect);
  }
}

void Renderer::DrawCircle(int cx, int cy, int radius, const SDL_Color &color) const {
  SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; ++w) {
    for (int h = 0; h < radius * 2; ++h) {
      const int dx = radius - w;
      const int dy = radius - h;
      if ((dx * dx + dy * dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(renderer_, cx + dx, cy + dy);
      }
    }
  }
}

}  // namespace vita::ui
