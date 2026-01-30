#include "scenes/home_screen.h"

#include <algorithm>
#include <string>

#include "ui/constants.h"

namespace vita::scenes {

HomeScreen::HomeScreen(const data::Library &library, data::RuntimeState &state)
    : library_(library), state_(state) {}

void HomeScreen::HandleEvent(const InputEvent &event) {
  if (event.type == InputEvent::Type::kTouchHold) {
    edit_mode_ = true;
  }
  if (event.type == InputEvent::Type::kKey && event.key) {
    std::string key(event.key);
    if (key == "left") {
      focused_index_ = std::max(0, focused_index_ - 1);
    } else if (key == "right") {
      focused_index_ += 1;
    } else if (key == "back") {
      edit_mode_ = false;
    }
  }
}

void HomeScreen::Update(int dt_ms) {
  if (toast_timer_ms_ > 0) {
    toast_timer_ms_ = std::max(0, toast_timer_ms_ - dt_ms);
    if (toast_timer_ms_ == 0) {
      toast_message_.clear();
    }
  }
}

void HomeScreen::Render(ui::Renderer &renderer) {
  renderer.Clear(ui::kColorBackground);
  renderer.DrawRect(0, 0, ui::kBaseWidth, ui::kInfoBarHeight, ui::kColorPanel);
  RenderPageDots(renderer);
  if (!toast_message_.empty()) {
    renderer.DrawRect(ui::kBaseWidth - 280, ui::kInfoBarHeight + 12, 260, 40, ui::kColorPanel);
  }
  if (edit_mode_) {
    renderer.DrawRectOutline(12, ui::kInfoBarHeight + 8, ui::kBaseWidth - 24, ui::kBaseHeight - 80,
                             ui::kColorFocus, 2);
  }
}

void HomeScreen::RenderPageDots(ui::Renderer &renderer) {
  const size_t total_pages = std::min(state_.pages.size(), static_cast<size_t>(ui::kMaxPages));
  if (total_pages == 0) {
    return;
  }
  const int start_x = (ui::kBaseWidth - static_cast<int>((total_pages - 1) * ui::kPageDotSpacing)) / 2;
  for (size_t index = 0; index < total_pages; ++index) {
    const SDL_Color color = (static_cast<int>(index) == state_.current_page) ? ui::kColorDotActive
                                                                             : ui::kColorDotInactive;
    renderer.DrawCircle(start_x + static_cast<int>(index) * ui::kPageDotSpacing, ui::kPageDotY,
                        ui::kPageDotRadius, color);
  }
}

void HomeScreen::ShowNotificationToast(std::string message, int duration_ms) {
  toast_message_ = std::move(message);
  toast_timer_ms_ = duration_ms;
}

}  // namespace vita::scenes
