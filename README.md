# RG35XX Game Dev

Minimal GBA homebrew project targeting the Anbernic RG35XX Plus (stock firmware, GBA emulator).

## Toolchain

- [devkitPro](https://devkitpro.org/) devkitARM (`arm-none-eabi-gcc` 16.1.0) + libgba
- Installed via MSYS2 pacman: `pacman -S gba-dev`

## Build

From an MSYS2 shell (or any shell with `DEVKITPRO`/`DEVKITARM`/`PATH` set — see below):

```sh
make
```

Produces `game.gba` (and `game.elf`) in the project root.

```sh
make clean
```

Removes build artifacts.

## Environment

`DEVKITPRO` and `DEVKITARM` must be set (persisted as Windows user env vars by the installer):

```
DEVKITPRO=/opt/devkitpro
DEVKITARM=/opt/devkitpro/devkitARM
```

## Structure

```
src/       game source (.c)
include/   headers
assets/    art/audio source, converted at build time
build/     intermediate object files (gitignored)
tools/     helper scripts
Makefile   devkitPro gba_rules-based build
```

## Status

`src/main.c` currently shows a procedural "WELCOME KHALED" title screen (starfield, mountains, sliding title, blinking prompt) and transitions to a placeholder "GAME STARTED" screen on START. No gameplay yet.

## PC testing (VS Code + mGBA)

For fast iteration without hardware, this repo has VS Code tasks that build and run the ROM in [mGBA](https://mgba.io/) (installed via `winget install JeffreyPfau.mGBA`):

- **Terminal → Run Task → Build Game** (or `Ctrl+Shift+B`) — runs `make` via devkitPro's own MSYS2 shell.
- **Terminal → Run Task → Run Game** (or `Ctrl+Alt+R`) — builds, verifies `game.gba` was produced, and opens it in mGBA.

See `.vscode/tasks.json` / `.vscode/keybindings.json`. This is a PC dev-loop convenience only — the RG35XX Plus remains the real hardware target (see below).

## Testing on hardware

Copy `game.gba` to the ROMs folder on the RG35XX Plus's SD card and launch it with the stock GBA emulator.
