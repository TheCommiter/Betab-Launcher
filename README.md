# Betab Launcher – Vita-Style Shell (C++/SDL2)

This repository contains a C++/SDL2 scaffold for a PS Vita–style UI shell. The implementation is data-driven via JSON and targets a virtual 960×544 canvas that scales to any window size with letterboxing.

## What’s Included

- **CMake build** for the C++ SDL2 application.
- **Scene stack** with Home, LiveArea, Notifications, Index, and Quick Menu overlay scaffolding.
- **Data model** using `data/library.json` and `data/state.json`.
- **Virtual canvas** helpers for aspect-preserving scaling.

## Build & Run

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
./vita_shell
```

## Data Files

- `data/library.json`: App/game metadata.
- `data/state.json`: Persisted runtime state (pages, folders, notifications, etc.).

## Notes

- The current implementation is a scaffold; UI layout and interactions are placeholders for future expansion.
- Rendering is decoupled via the virtual canvas to allow future portability.
