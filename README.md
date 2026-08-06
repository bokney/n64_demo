# Hello World — N64 Homebrew

Nintendo 64 homebrew game template built with **libdragon**

## Features

- **Fixed-timestep game loop**
- **Phase-based state machine**
- **ECS architecture**
- **RDPQ rendering**
- **Decoupled input system**
- **XM64 music streaming**

## Tech Stack

| Layer | Technology |
|---|---|
| Toolchain | `mipsel-none-elf-gcc` via libdragon |
| Graphics | RDPQ (libdragon) |
| Audio | libdragon mixer + XM64 player |
| Build | Makefile + `n64.mk` |
| Language | C99 (libdragon) |

## Building

```bash
libdragon make clean
libdragon make
```

Requires the libdragon submodule and N64 toolchain installed. See libdragon's README for setup.

## Project Structure

```
src/
  main.c            — engine entry point, fixed-timestep loop
  game.h            — game-specific types (StateID, etc.)
  state.c/h         — phase-based state machine
  ecs.c/h           — ECS: entity management, component storage, systems
  systems/
    input.c/h       — input abstraction, deadzones, edge detection
    render.c/h      — RDPQ rendering system, queries ECS for draw data
  states/
    splash_screen.* — splash screen with animated box + music
    title_screen.*  — title screen skeleton
    main_menu.*     — main menu skeleton
filesystem/         — game assets (audio, sprites)
```
