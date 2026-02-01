#pragma once

#include <string>

#include "data/library.h"
#include "data/state.h"
#include "scenes/scene.h"

namespace vita::scenes {

class HomeScreen : public Scene {
 public:
  HomeScreen(const data::Library &library, data::RuntimeState &state);

  void HandleEvent(const InputEvent &event) override;
  void Update(int dt_ms) override;
  void Render(ui::Renderer &renderer) override;

  void ShowNotificationToast(std::string message, int duration_ms);

 private:
  const data::Library &library_;
  data::RuntimeState &state_;
  int focused_index_ = 0;
  bool edit_mode_ = false;
  int toast_timer_ms_ = 0;
  std::string toast_message_;

  void RenderPageDots(ui::Renderer &renderer);
};

}  // namespace vita::scenes
