#pragma once

#include "data/library.h"
#include "data/state.h"
#include "scenes/scene.h"

namespace vita::scenes {

class LiveAreaScreen : public Scene {
 public:
  LiveAreaScreen(const data::LibraryItem &item, data::RuntimeState &state);

  void HandleEvent(const InputEvent &event) override;
  void Update(int dt_ms) override;
  void Render(ui::Renderer &renderer) override;

 private:
  const data::LibraryItem &item_;
  data::RuntimeState &state_;
  bool launching_ = false;
};

}  // namespace vita::scenes
