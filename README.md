# YAQUB: THE SLEEPY GUARDIANS OF THE NILE (SNES)

A 2D pixel-art action platformer in the Game Boy Color tradition, built
on native SNES hardware with PVSnesLib and targeting the Anbernic RG35XX
Plus's SNES emulator core. Developed and tested on PC first.

You play Yaqub, a Bengal-marked warrior cat armed with the Moon Claw, who
is much more interested in sleeping than in saving the Kingdom of Qamar.

## Status

First vertical slice: title screen -> intro -> Level 1 (The Forgotten
Oasis) -> move / jump / crescent-slash attack -> scarab + snake + tiny
mummy + desert rat enemies -> health -> the Golden Scorpion boss. See
`CLAUDE.md` for full design direction and what's deliberately not built
yet.

## Toolchain

- [PVSnesLib](https://github.com/alekmaul/pvsneslib) 4.6.0 -- C compiler
  (`816-tcc`), assembler/linker (`wla-65816` / `wlalink`), and the
  `gfx4snes` PNG-to-native-tile/palette converter for the 65816.
- Installed to `C:/snesdev` (the `PVSNESLIB_HOME` env var, set as a
  persistent Windows user variable, points here).
- Uses devkitPro's bundled MSYS2 (`C:\devkitPro\msys2`) for `bash`/`make`.

**Important Windows-specific gotcha**: `PVSNESLIB_HOME` must be a
Windows-style path with forward slashes (`C:/snesdev`), *not* the
Unix-mount style (`/c/snesdev`) some PVSnesLib docs show. The bundled
`wlalink.exe`/`816-tcc.exe` are native (non-MSYS) binaries and can't
resolve `/c/...` paths passed as arguments -- only `C:/...` ones.

## Build

From an MSYS2 shell with `PVSNESLIB_HOME` and `PATH` set (or via the VS
Code "Build Game" task, which does this for you):

```sh
export PVSNESLIB_HOME=C:/snesdev
export PATH=$PVSNESLIB_HOME/devkitsnes/bin:$PVSNESLIB_HOME/devkitsnes/tools:$PATH
make
```

Produces `Yaqub.sfc` in the project root.

```sh
make clean
```

Removes build artifacts, including the `assets/**/*.pic|.pal|.map|.inc`
files generated from source `.png` art.

## PC testing (VS Code + Mesen2)

- **Terminal -> Run Task -> Build Game** (`Ctrl+Shift+B`) -- builds
  `Yaqub.sfc`.
- **Terminal -> Run Task -> Run Game** (`Ctrl+Alt+R`) -- builds, then
  launches it in [Mesen2](https://www.mesen.ca/) (installed via
  `winget install SourMesen.Mesen2`).

## Controls

D-PAD move, A jump, B crescent-slash attack, START pause.

## Structure

```
src/
    main.c          state-machine entry point / frame loop
    game_state.c/h  TITLE / INTRO / GAMEPLAY states
    input.c/h       pad wrapper
    camera.c/h      horizontal scroll follow-cam
    level.c/h + level1_data.h   tilemap + per-tile collision table
    collision.c/h   AABB-vs-tile-grid queries
    player.c/h      Yaqub: physics, animation state machine, attack
    enemy.c/h       scarab / snake / tiny mummy / desert rat / Golden
                    Scorpion boss AI (boss drawn as 4 composited quadrants)
    combat.c/h      hitboxes, hurtboxes, health, damage
    text.c/h        generic BG1 bitmap-font renderer (HUD + intro text)
assets/
    player/         yaqub.png (32x32 frame grid), crescent_slash.png (16x16)
    enemies/        scarab/snake/mummy/rat.png (32x32 frame grid),
                    boss_scorpion.png (32x32 quadrants, 4 per 64x64 state)
    levels/         level1_tileset.png (16x16 authored tiles, stored as
                    2x2 blocks of 8x8 engine tiles -- see CLAUDE.md)
    ui/font/        runtime bitmap font
    ui/title/       title.png (full 256x224 picture)
    ui/intro/       intro.png (full 256x224 picture)
data.asm    hand-written WLA-DX wrapper that incbins converted art
Makefile    PVSnesLib snes_rules-based build; ROMNAME=Yaqub
```

Art is currently programmatically generated placeholder pixel art (see
`CLAUDE.md` -> "Current art pipeline") -- readable and on-style, not
hand-crafted. Drop in replacement PNGs at the same pixel dimensions/frame
grid to upgrade it without touching the build pipeline.

## Testing on hardware

Copy `Yaqub.sfc` to the ROMs folder on the RG35XX Plus's SD card and
launch it with the stock SNES emulator core.
