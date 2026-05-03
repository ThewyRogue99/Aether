<div align="center">
  <img src="assets/icons/aether-blue.png" alt="Aether Engine" width="120"/>

  # Aether Engine

  A C++ game engine built for simplicity and power.

  ![C++](https://img.shields.io/badge/C%2B%2B-20-blue?style=flat-square)
  ![CMake](https://img.shields.io/badge/CMake-3.16+-blue?style=flat-square)
  ![OpenGL](https://img.shields.io/badge/Renderer-OpenGL-orange?style=flat-square&logo=opengl)
  ![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey?style=flat-square)
  ![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)
</div>

---

## Overview

Aether is a modular, data-driven game engine written in modern C++20. It is designed with a clean architecture that keeps simplicity and extensibility at its core, with a clear path toward Vulkan rendering.

## Features

- **Entity Component System** — lightweight, cache-friendly ECS at the heart of the engine
- **OpenGL Renderer** — abstracted rendering backend with a Vulkan-ready API surface
- **File-based Shader Pipeline** — shaders authored in a single `.glsl` file split by `#pragma vertex` / `#pragma fragment`; pipeline state defined in `.pipeline` JSON files
- **Scene Management** — load, unload, and switch scenes at runtime
- **Editor** — built on Dear ImGui with docking, a 3D viewport, scene hierarchy, and properties panels
- **Layer System** — ordered layer stack for game, editor, and UI layers
- **Event System** — decoupled event dispatch across the engine
- **Logging** — structured, leveled logging via a custom logger (spdlog-backed)
- **Cross-platform** — Windows, macOS, and Linux

## Tech Stack

| Dependency | Role |
|---|---|
| [GLFW](https://github.com/glfw/glfw) | Windowing & input |
| [glad](https://github.com/Dav1dde/glad) | OpenGL loader |
| [glm](https://github.com/g-truc/glm) | Math library |
| [Dear ImGui](https://github.com/ocornut/imgui) | Editor UI |
| [spdlog](https://github.com/gabime/spdlog) | Logging backend |
| [nlohmann/json](https://github.com/nlohmann/json) | Pipeline file parsing (internal) |

All dependencies are fetched automatically at configure time via CMake `FetchContent`.

## Building

**Requirements:** CMake 3.16+, a C++20 compiler (GCC 11+, Clang 13+, or MSVC 2022+), Git.

```bash
git clone https://github.com/ThewyRogue99/Aether.git
cd Aether

# Debug
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug

# Release
cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release
```

The `Editor` binary and its assets are placed next to each other in the build output directory automatically.

## Project Structure

```
Aether/
├── Engine/                 # Core engine library (shared)
│   ├── include/Aether/     # Public headers
│   └── src/
│       ├── Core/           # Application, layers, logging, string, JSON wrapper
│       ├── ECS/            # Entity Component System
│       ├── Events/         # Event types and dispatch
│       ├── Input/          # Keyboard & mouse input
│       ├── Math/           # Vector, matrix, math utilities
│       ├── Platform/       # Window, graphics context
│       ├── Renderer/       # Render backend, pipeline loader, shader splitter
│       ├── Scene/          # Scene and scene manager
│       └── Systems/        # Built-in systems (rendering, movement)
├── Editor/                 # Editor application
│   ├── src/                # Editor layers, panels, themes
│   └── assets/
│       ├── fonts/          # UI fonts
│       └── shaders/        # .glsl + .pipeline asset files
├── Sandbox/                # Scratch application for engine testing
├── external/               # CMake FetchContent wrappers for dependencies
└── assets/                 # Shared assets (icons, etc.)
```

## Roadmap

- [x] OpenGL rendering backend
- [x] Entity Component System
- [x] Editor with ImGui docking
- [x] File-based shader & pipeline system
- [ ] Asset manager
- [ ] Vulkan rendering backend
- [ ] Physics integration
- [ ] Scripting layer

## License

MIT — see [LICENSE](LICENSE) for details.
