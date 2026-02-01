#pragma once

#include "data/state.h"
#include "scenes/scene.h"

namespace vita::scenes {

class IndexScreen : public Scene {
 public:
  explicit IndexScreen(data::RuntimeState &state);

  void HandleEvent(const InputEvent &event) override;
  void Update(int dt_ms) override;
  void Render(ui::Renderer &renderer) override;
  bool IsVisible() const override { return visible_; }
  bool AcceptsInput() const override { return visible_; }

  void Toggle() { visible_ = !visible_; }
  void SetVisible(bool visible) { visible_ = visible; }

 private:
  data::RuntimeState &state_;
  bool visible_ = false;
};

}  // namespace vita::scenes
