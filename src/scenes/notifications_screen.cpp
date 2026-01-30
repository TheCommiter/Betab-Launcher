#include "scenes/notifications_screen.h"

#include <string>

#include "ui/constants.h"

namespace vita::scenes {

NotificationsScreen::NotificationsScreen(data::RuntimeState &state) : state_(state) {}

void NotificationsScreen::HandleEvent(const InputEvent &event) {
  if (!visible_) {
    return;
  }
  if (event.type == InputEvent::Type::kKey && event.key && std::string(event.key) == "back") {
    visible_ = false;
  }
}

void NotificationsScreen::Update(int /*dt_ms*/) {}

void NotificationsScreen::Render(ui::Renderer &renderer) {
  if (!visible_) {
    return;
  }
  renderer.DrawRect(80, 80, ui::kBaseWidth - 160, ui::kBaseHeight - 160, ui::kColorPanel);
}

void NotificationsScreen::Toggle() {
  visible_ = !visible_;
  if (visible_) {
    state_.notifications.clear();
  }
}

}  // namespace vita::scenes
