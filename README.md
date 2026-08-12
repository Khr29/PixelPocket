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

Current `src/main.c` is a minimal pipeline test: fills the screen with a solid color. No gameplay yet — this exists to prove compile → link → `.gba` → hardware works end to end.

## Testing on hardware

Copy `game.gba` to the ROMs folder on the RG35XX Plus's SD card and launch it with the stock GBA emulator.
