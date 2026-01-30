cmake_minimum_required(VERSION 3.16)
project(vita_shell LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(SDL2 REQUIRED)

add_executable(vita_shell
  src/main.cpp
  src/data/json.cpp
  src/data/library.cpp
  src/data/state.cpp
  src/scenes/scene_stack.cpp
  src/scenes/home_screen.cpp
  src/scenes/livearea_screen.cpp
  src/scenes/notifications_screen.cpp
  src/scenes/index_screen.cpp
  src/scenes/overlays.cpp
  src/ui/layout.cpp
  src/ui/renderer.cpp
)

target_include_directories(vita_shell PRIVATE src)
target_link_libraries(vita_shell PRIVATE SDL2::SDL2)
