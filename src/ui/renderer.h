#pragma once

#include <SDL.h>

namespace vita::ui {

class Renderer {
 public:
  explicit Renderer(SDL_Renderer *renderer);

  void Clear(const SDL_Color &color) const;
  void DrawRect(int x, int y, int w, int h, const SDL_Color &color) const;
  void DrawCircle(int cx, int cy, int radius, const SDL_Color &color) const;
  void DrawRectOutline(int x, int y, int w, int h, const SDL_Color &color, int thickness = 1) const;

 private:
  SDL_Renderer *renderer_;
};

}  // namespace vita::ui
