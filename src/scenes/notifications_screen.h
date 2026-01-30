#pragma once

#include "data/state.h"
#include "scenes/scene.h"

namespace vita::scenes {

class NotificationsScreen : public Scene {
 public:
  explicit NotificationsScreen(data::RuntimeState &state);

  void HandleEvent(const InputEvent &event) override;
  void Update(int dt_ms) override;
  void Render(ui::Renderer &renderer) override;
  bool IsVisible() const override { return visible_; }
  bool AcceptsInput() const override { return visible_; }

  void Toggle();
  void SetVisible(bool visible) { visible_ = visible; }
  bool visible() const { return visible_; }

 private:
  data::RuntimeState &state_;
  bool visible_ = false;
};

}  // namespace vita::scenes
