#pragma once

#include <SDL.h>

namespace vita::ui {

struct Letterbox {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
};

class VirtualCanvas {
 public:
  Letterbox ComputeLetterbox(int window_width, int window_height) const;
  SDL_FPoint ToVirtual(const SDL_FPoint &screen_point, const Letterbox &letterbox) const;
  SDL_FPoint ToScreen(const SDL_FPoint &virtual_point, const Letterbox &letterbox) const;
};

}  // namespace vita::ui
