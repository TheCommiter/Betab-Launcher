#include "ui/layout.h"

#include "ui/constants.h"

namespace vita::ui {

Letterbox VirtualCanvas::ComputeLetterbox(int window_width, int window_height) const {
  const float scale_x = static_cast<float>(window_width) / static_cast<float>(kBaseWidth);
  const float scale_y = static_cast<float>(window_height) / static_cast<float>(kBaseHeight);
  const float scale = (scale_x < scale_y) ? scale_x : scale_y;
  const int width = static_cast<int>(kBaseWidth * scale);
  const int height = static_cast<int>(kBaseHeight * scale);
  const int x = (window_width - width) / 2;
  const int y = (window_height - height) / 2;
  return Letterbox{x, y, width, height};
}

SDL_FPoint VirtualCanvas::ToVirtual(const SDL_FPoint &screen_point, const Letterbox &letterbox) const {
  SDL_FPoint result{0.0f, 0.0f};
  if (letterbox.width == 0 || letterbox.height == 0) {
    return result;
  }
  result.x = (screen_point.x - static_cast<float>(letterbox.x)) * kBaseWidth /
             static_cast<float>(letterbox.width);
  result.y = (screen_point.y - static_cast<float>(letterbox.y)) * kBaseHeight /
             static_cast<float>(letterbox.height);
  return result;
}

SDL_FPoint VirtualCanvas::ToScreen(const SDL_FPoint &virtual_point, const Letterbox &letterbox) const {
  SDL_FPoint result{0.0f, 0.0f};
  result.x = static_cast<float>(letterbox.x) +
             (virtual_point.x * static_cast<float>(letterbox.width) / kBaseWidth);
  result.y = static_cast<float>(letterbox.y) +
             (virtual_point.y * static_cast<float>(letterbox.height) / kBaseHeight);
  return result;
}

}  // namespace vita::ui
