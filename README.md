# YAQUB: THE SLEEPY GUARDIANS OF THE NILE

A compact SNES pixel-art action platformer targeting the Anbernic RG35XX Plus.

## Current vertical slice

- Title screen
- Opening intro
- The Forgotten Oasis
- Yaqub movement and jumping
- Crescent Claw attack
- Scarab
- Desert Snake
- Tiny Mummy
- Desert Rat
- Golden Scorpion boss
- Horizontal camera scrolling
- Tile collision

## Visual direction

The supplied Yaqub/Forgotten Oasis art package is the visual source of truth.
The runtime artwork uses the same characters, Egyptian night environment,
moonlight, colors, sprites, and chunky pixel treatment, adapted to native SNES
4bpp tiles and sprite limits.

## Toolchain

- PVSnesLib 4.6.0
- 816-tcc
- wla-65816 / wlalink
- gfx4snes
- devkitPro MSYS2
- Mesen2

PVSnesLib is expected at `C:/snesdev` through `PVSNESLIB_HOME`.

## Build

From devkitPro's MSYS2 shell:

```sh
export PVSNESLIB_HOME=C:/snesdev
export PATH=$PVSNESLIB_HOME/devkitsnes/bin:$PVSNESLIB_HOME/devkitsnes/tools:$PATH
make clean
make
```

The ROM is produced as:

```text
Yaqub.sfc
```

## PC test

Open `Yaqub.sfc` in Mesen2.

Controls:

```text
D-pad   Move
A       Jump
B       Crescent Slash
START   Advance / skip intro
```

## Project structure

```text
src/
  main.c
  game_state.c/h
  input.c/h
  camera.c/h
  level.c/h
  level1_data.h        collision/spawn data
  level_bg_map.h       reference-derived visual map
  collision.c/h
  player.c/h
  enemy.c/h
  combat.c/h

assets/
  player/
    yaqub.png
    crescent_slash.png
  enemies/
    scarab.png
    snake.png
    mummy.png
    rat.png
    boss_scorpion.png
  levels/
    level_bg_tileset.png
  ui/
    title/title.png
    intro/intro.png

data.asm
Makefile
CLAUDE.md
```

## SNES VRAM layout

```text
0x0000–0x37FF   dynamic large sprites
0x3800–0x3BFF   dynamic small sprites
0x4000–0x77FF   BG0 tiles
0x7800–0x7FFF   BG0 64×32 map
```

BG tile bases must stay 4K-word aligned.

## Design rule

Do not replace the supplied Forgotten Oasis visual direction with generic
placeholder graphics. New artwork should be designed to match the existing
reference-derived assets and remain usable on the RG35XX Plus.
