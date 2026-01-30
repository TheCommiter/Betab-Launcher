#pragma once

#include "ui/renderer.h"

namespace vita::scenes {

struct InputEvent {
  enum class Type {
    kKey,
    kTouchHold,
  };

  Type type;
  const char *key = nullptr;
};

class Scene {
 public:
  virtual ~Scene() = default;
  virtual void HandleEvent(const InputEvent &event) = 0;
  virtual void Update(int dt_ms) = 0;
  virtual void Render(ui::Renderer &renderer) = 0;
  virtual bool IsVisible() const { return true; }
  virtual bool AcceptsInput() const { return IsVisible(); }
};

}  // namespace vita::scenes
