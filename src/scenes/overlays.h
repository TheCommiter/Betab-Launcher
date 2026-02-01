#pragma once

#include "scenes/scene.h"

namespace vita::scenes {

class QuickMenuOverlay : public Scene {
 public:
  void HandleEvent(const InputEvent &event) override;
  void Update(int dt_ms) override;
  void Render(ui::Renderer &renderer) override;
  bool IsVisible() const override { return visible_; }
  bool AcceptsInput() const override { return visible_; }

  void SetVisible(bool visible) { visible_ = visible; }
  bool visible() const { return visible_; }

 private:
  bool visible_ = false;
};

}  // namespace vita::scenes
