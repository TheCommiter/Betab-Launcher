#include "scenes/scene_stack.h"

namespace vita::scenes {

void SceneStack::Push(Scene *scene) {
  stack_.push_back(scene);
}

void SceneStack::Pop() {
  if (!stack_.empty()) {
    stack_.pop_back();
  }
}

void SceneStack::HandleEvent(const InputEvent &event) {
  for (auto iter = stack_.rbegin(); iter != stack_.rend(); ++iter) {
    if ((*iter)->AcceptsInput()) {
      (*iter)->HandleEvent(event);
      break;
    }
  }
}

void SceneStack::Update(int dt_ms) {
  for (auto *scene : stack_) {
    scene->Update(dt_ms);
  }
}

void SceneStack::Render(ui::Renderer &renderer) {
  for (auto *scene : stack_) {
    if (scene->IsVisible()) {
      scene->Render(renderer);
    }
  }
}

}  // namespace vita::scenes
