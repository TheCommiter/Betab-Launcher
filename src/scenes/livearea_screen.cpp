#include "scenes/livearea_screen.h"

#include <chrono>
#include <cstdlib>
#include <string>

#include "ui/constants.h"

namespace vita::scenes {

LiveAreaScreen::LiveAreaScreen(const data::LibraryItem &item, data::RuntimeState &state)
    : item_(item), state_(state) {}

void LiveAreaScreen::HandleEvent(const InputEvent &event) {
  if (event.type == InputEvent::Type::kKey && event.key && std::string(event.key) == "accept") {
    launching_ = true;
    state_.last_played[item_.item_id] =
        std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
  }
}

void LiveAreaScreen::Update(int /*dt_ms*/) {
  if (launching_ && !item_.cmd_linux.empty()) {
    std::string command;
    for (const auto &part : item_.cmd_linux) {
      if (!command.empty()) {
        command += " ";
      }
      command += part;
    }
    std::system(command.c_str());
    launching_ = false;
  }
}

void LiveAreaScreen::Render(ui::Renderer &renderer) {
  renderer.Clear(ui::kColorBackground);
  const int hero_x = (ui::kBaseWidth - ui::kHeroWidth) / 2;
  const int hero_y = 80;
  renderer.DrawRect(hero_x, hero_y, ui::kHeroWidth, ui::kHeroHeight, ui::kColorPanel);
  renderer.DrawRect((ui::kBaseWidth - ui::kGateButtonWidth) / 2, hero_y + ui::kHeroHeight + 20,
                    ui::kGateButtonWidth, ui::kGateButtonHeight, ui::kColorFocus);
}

}  // namespace vita::scenes
