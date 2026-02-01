#include <SDL.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <optional>

#include "data/library.h"
#include "data/state.h"
#include "scenes/home_screen.h"
#include "scenes/index_screen.h"
#include "scenes/notifications_screen.h"
#include "scenes/overlays.h"
#include "scenes/scene_stack.h"
#include "ui/constants.h"
#include "ui/layout.h"
#include "ui/renderer.h"

namespace {

std::vector<std::vector<std::string>> BuildDefaultPages(const vita::data::Library &library) {
  std::vector<std::vector<std::string>> pages{std::vector<std::string>{}};
  for (const auto &item : library.items()) {
    if (pages.back().size() >= 15) {
      pages.emplace_back();
    }
    pages.back().push_back(item.item_id);
  }
  return pages;
}

}  // namespace

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
    std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Vita Shell", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 1280, 720,
                                        SDL_WINDOW_RESIZABLE);
  if (!window) {
    std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_Texture *offscreen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET,
                                             vita::ui::kBaseWidth, vita::ui::kBaseHeight);

  vita::data::Library library = vita::data::Library::Load("data/library.json");
  vita::data::StateStore state_store(std::filesystem::path("data/state.json"));
  vita::data::RuntimeState state = state_store.Load();
  if (state.pages.empty()) {
    state.pages = BuildDefaultPages(library);
  }
  try {
    state.EnsureLimits(library.items().size());
  } catch (const std::exception &error) {
    std::cerr << "State invalid: " << error.what() << "\n";
  }

  vita::scenes::HomeScreen home(library, state);
  vita::scenes::NotificationsScreen notifications(state);
  vita::scenes::IndexScreen index_screen(state);
  vita::scenes::QuickMenuOverlay quick_menu;

  vita::scenes::SceneStack stack;
  stack.Push(&home);
  stack.Push(&notifications);
  stack.Push(&index_screen);
  stack.Push(&quick_menu);

  vita::ui::Renderer render(renderer);
  vita::ui::VirtualCanvas canvas;

  bool running = true;
  std::optional<std::chrono::steady_clock::time_point> home_down;
  std::optional<std::chrono::steady_clock::time_point> touch_down;

  auto last_time = std::chrono::steady_clock::now();

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
        break;
      }
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            stack.HandleEvent({vita::scenes::InputEvent::Type::kKey, "back"});
            break;
          case SDLK_RETURN:
            stack.HandleEvent({vita::scenes::InputEvent::Type::kKey, "accept"});
            break;
          case SDLK_LEFT:
            stack.HandleEvent({vita::scenes::InputEvent::Type::kKey, "left"});
            break;
          case SDLK_RIGHT:
            stack.HandleEvent({vita::scenes::InputEvent::Type::kKey, "right"});
            break;
          case SDLK_UP:
            stack.HandleEvent({vita::scenes::InputEvent::Type::kKey, "up"});
            break;
          case SDLK_DOWN:
            stack.HandleEvent({vita::scenes::InputEvent::Type::kKey, "down"});
            break;
          case SDLK_SPACE:
            if (!home_down) {
              home_down = std::chrono::steady_clock::now();
            }
            break;
          case SDLK_TAB:
            notifications.Toggle();
            break;
          case SDLK_n:
            state.notifications.push_back({"New trophy unlocked", ""});
            home.ShowNotificationToast("New notification", vita::ui::kNotificationToastMs);
            break;
          default:
            break;
        }
      }
      if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
        if (home_down) {
          auto elapsed =
              std::chrono::duration<double>(std::chrono::steady_clock::now() - *home_down);
          if (elapsed.count() <= 0.6 && !quick_menu.visible()) {
            index_screen.Toggle();
          }
          home_down.reset();
        }
      }
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        touch_down = std::chrono::steady_clock::now();
      }
      if (event.type == SDL_MOUSEBUTTONUP) {
        if (touch_down) {
          const auto elapsed =
              std::chrono::duration<double>(std::chrono::steady_clock::now() - *touch_down);
          if (elapsed.count() > 0.45) {
            stack.HandleEvent({vita::scenes::InputEvent::Type::kTouchHold, nullptr});
          }
          touch_down.reset();
        }
      }
    }

    if (home_down) {
      auto elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - *home_down);
      if (elapsed.count() > 0.6) {
        quick_menu.SetVisible(true);
        home_down.reset();
      }
    }

    auto now = std::chrono::steady_clock::now();
    const int dt_ms =
        static_cast<int>(std::chrono::duration<double, std::milli>(now - last_time).count());
    last_time = now;

    stack.Update(dt_ms);

    SDL_SetRenderTarget(renderer, offscreen);
    stack.Render(render);
    SDL_SetRenderTarget(renderer, nullptr);

    int win_w = 0;
    int win_h = 0;
    SDL_GetWindowSize(window, &win_w, &win_h);
    vita::ui::Letterbox letterbox = canvas.ComputeLetterbox(win_w, win_h);
    SDL_Rect dst{letterbox.x, letterbox.y, letterbox.width, letterbox.height};
    SDL_RenderCopy(renderer, offscreen, nullptr, &dst);
    SDL_RenderPresent(renderer);
  }

  state_store.Save(state);
  SDL_DestroyTexture(offscreen);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
