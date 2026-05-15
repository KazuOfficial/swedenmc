# SwedenMC

A C port of Minecraft Beta 1.7.3. **[swedenmc.net](https://swedenmc.net)**

> **Experimental.** Crashes, missing features, and broken behaviour are expected. This is a work in progress.

For questions and bug reports: **[Discord](https://discord.gg/tGFPh72Bs6)**

---

## Features

- [x] Skins
- [x] Sound
- [x] Online mode
- [x] Improved renderer
- [x] Multithreading

## Platform support

| Platform | Status |
|---|---|
| macOS (Apple Silicon) | Tested |
| macOS (Intel) | Untested |
| Linux (x64) | Untested |
| Linux (ARM64) | Tested |
| Windows (x64) | Tested |
| Windows (ARM64) | Untested |
| Browser (WebAssembly) | Experimental |
| iOS | Planned |
| Android | Planned |
| Nintendo Wii | Planned |
| Nintendo 3DS | Planned |
| PlayStation Portable | Planned |

## Renderer support

Two rendering backends are available and can be switched from the launcher settings:

- **OpenGL** — fixed-function OpenGL 2.1
- **bgfx** — cross-platform rendering via [bgfx](https://github.com/bkaradzic/bgfx), supporting Metal (macOS/iOS), Vulkan (Windows/Linux/Android), Direct3D 11/12 (Windows), and OpenGL ES (mobile)

## Building

```sh
git submodule update --init --recursive
make
```

Requires: Clang, CMake, curl, minizip, zlib.

bgfx and GLFW are built from source as part of the build. On macOS, install dependencies via Homebrew:

```sh
brew install cmake minizip curl
```

## Libraries

- [bgfx](https://github.com/bkaradzic/bgfx) — cross-platform graphics (BSD 2-Clause)
- [bx](https://github.com/bkaradzic/bx) — base library for bgfx (BSD 2-Clause)
- [bimg](https://github.com/bkaradzic/bimg) — image library for bgfx (BSD 2-Clause)
- [GLFW](https://www.glfw.org/) — windowing and input (zlib)
- [glad](https://github.com/Dav1dde/glad) — OpenGL loader (MIT)
- [openal-soft](https://github.com/kcat/openal-soft) — audio (LGPL-2.0)
- [stb](https://github.com/nothings/stb) — image loading and vorbis decoding (MIT/Public Domain)

## License

Fan-made preservation port of Minecraft Beta 1.7.3, translated from decompiled Java source. Not affiliated with or endorsed by Mojang or Microsoft. Minecraft is the intellectual property of Mojang AB. Shared for educational and preservation purposes only.
