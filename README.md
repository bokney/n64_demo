# Hello World — N64 Homebrew

Nintendo 64 homebrew game template built with **libdragon**.

## Features

- **Fixed-timestep game loop** (60 Hz logic, decoupled render)
- **Phase-based state machine** (splash → title → menu → gameplay → game over)
- **ECS architecture** (entity creation, component storage, logic systems)
- **RDPQ + tiny3d rendering** (3D models, depth-buffered, lighting, camera)
- **Decoupled input system** (action mapping, held/pressed/released)

## Tech Stack

| Layer | Technology |
|---|---|
| Toolchain | `mipsel-none-elf-gcc` via libdragon |
| Graphics | RDPQ + tiny3d |
| Audio | libdragon mixer + XM64 player |
| Build | Makefile + `n64.mk` |
| Testing | Unity + host-compiled mocks |
| Language | C17 (gnu17, libdragon) |

## Building

```bash
libdragon make clean
libdragon make
```

Requires the libdragon submodule and N64 toolchain installed.

## Testing

```bash
make -C test test
```

Unit tests run on the host using [Unity](https://github.com/ThrowTheSwitch/Unity) with mock headers for `libdragon` and `t3d`, so no N64 hardware or emulator is needed.

## Project Structure

```
src/
  main.c                         — engine entry point, fixed-timestep loop
  game.h                         — StateID enum, shared game types
  state.{c,h}                    — phase-based state machine (init/update/exit)
  ecs.{c,h}                      — entity manager, component storage, logic dispatch
  ecs_components.h               — core entity/component types
   states/
     splash_screen.{c,h}          — splash with animated box + music
     title_screen.{c,h}           — title screen skeleton
     main_menu.{c,h}              — main menu
     gameplay.{c,h}               — gameplay loop
     game_over.{c,h}              — game over screen
     state_registry.{c,h}         — routes StateID to concrete state functions
  systems/
    input.{c,h}                  — action mapping, deadzone, edge detection
    input_components.{c,h}       — input component storage
    render.{c,h}                 — RDPQ drawing system, ECS render queries
    render_components.{c,h}      — renderable mesh/sprite components
    camera.{c,h}                 — 3D camera system (view/proj matrices)
    camera_components.{c,h}      — camera component storage
    lighting.{c,h}               — lighting system (apply to viewport)
    lighting_components.{c,h}    — light source components
assets/                          — source assets (models, audio, sprites)
  models/*.glb                   — 3D models → compiled to .t3dm
  audio/*.xm                     — XM tracks → compiled to .xm64
  sprites/*.png                  — sprites → compiled to .sprite
filesystem/                      — DFS root: compiled assets + raw files
test/                            — host-compiled Unity tests (state, ecs)
  mocks/                         — stub headers for libdragon and t3d
```
