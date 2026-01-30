#include "scenes/index_screen.h"

#include <string>

#include "ui/constants.h"

namespace vita::scenes {

IndexScreen::IndexScreen(data::RuntimeState &state) : state_(state) {}

void IndexScreen::HandleEvent(const InputEvent &event) {
  if (!visible_) {
    return;
  }
  if (event.type == InputEvent::Type::kKey && event.key && std::string(event.key) == "back") {
    visible_ = false;
  }
}

void IndexScreen::Update(int /*dt_ms*/) {}

void IndexScreen::Render(ui::Renderer &renderer) {
  if (!visible_) {
    return;
  }
  renderer.DrawRect(140, 120, ui::kBaseWidth - 280, ui::kBaseHeight - 240, ui::kColorPanel);
}

}  // namespace vita::scenes
