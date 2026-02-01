#pragma once

#include <vector>

#include "scenes/scene.h"

namespace vita::scenes {

class SceneStack {
 public:
  void Push(Scene *scene);
  void Pop();
  void HandleEvent(const InputEvent &event);
  void Update(int dt_ms);
  void Render(ui::Renderer &renderer);

 private:
  std::vector<Scene *> stack_;
};

}  // namespace vita::scenes
