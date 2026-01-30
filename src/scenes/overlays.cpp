#include "scenes/overlays.h"

#include <string>

#include "ui/constants.h"

namespace vita::scenes {

void QuickMenuOverlay::HandleEvent(const InputEvent &event) {
  if (event.type == InputEvent::Type::kKey && event.key && std::string(event.key) == "back") {
    visible_ = false;
  }
}

void QuickMenuOverlay::Update(int /*dt_ms*/) {}

void QuickMenuOverlay::Render(ui::Renderer &renderer) {
  if (!visible_) {
    return;
  }
  renderer.DrawRect(0, 0, ui::kBaseWidth, ui::kBaseHeight, ui::kColorScrim);
  renderer.DrawRect(200, 100, ui::kBaseWidth - 400, ui::kBaseHeight - 200, ui::kColorPanel);
}

}  // namespace vita::scenes
