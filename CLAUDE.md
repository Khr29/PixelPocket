# YAQUB — PROJECT RULES

## Project identity

This project is **YAQUB: THE SLEEPY GUARDIANS OF THE NILE**, a compact 2D
pixel-art action platformer built for native SNES hardware with PVSnesLib and
targeted at the Anbernic RG35XX Plus SNES emulator/core.

The visual source of truth is the supplied **Yaqub Forgotten Oasis art package**:
Yaqub sprite sheet, enemy sprites, Golden Scorpion, tiles, background layers,
and opening scene.

Do not replace that visual direction with generic procedural art.

## Current scope

One polished vertical slice:

Title → Intro → The Forgotten Oasis → Yaqub movement/jump/attack → enemies →
Golden Scorpion.

Do not add unrelated systems unless explicitly requested.

## Visual rules

- Chunky, crisp pixel art.
- Hard pixel edges; no anti-aliasing or blur.
- Dark navy/blue-green night palette with moonlit green, warm sand, stone,
and restrained gold accents.
- Yaqub is a spotted Bengal-like cat with the exact proportions/markings of
the supplied reference sprite sheet.
- The environment should match the supplied Forgotten Oasis reference as
closely as SNES hardware permits.
- Do not replace the reference-derived art with geometric placeholders.
- Use nearest-neighbor/integer scaling only.

## Asset source of truth

The current PNG assets were derived from the supplied art package and prepared
for the existing SNES frame layouts:

- `assets/player/yaqub.png` — 4×4 grid of 32×32 frames:
  idle×3, walk×4, jump, fall, attack×3, hurt, sleep.
- `assets/player/crescent_slash.png` — 2×16×16 effect frames.
- `assets/enemies/scarab.png`
- `assets/enemies/snake.png`
- `assets/enemies/mummy.png`
- `assets/enemies/rat.png`
- `assets/enemies/boss_scorpion.png` — 2×2 grid of 64×64 states, each
  represented by the engine as a 32×32 quadrant.
- `assets/levels/level_bg_tileset.png` — reference-derived 8×8 SNES tiles.
- `src/level_bg_map.h` — 64×32 visual map for the Forgotten Oasis.
- `assets/ui/title/title.png` — 256×224 title scene using the same visual
  language.
- `assets/ui/intro/intro.png` — 256×224 opening scene.

Every game asset must remain SNES-compatible: 4bpp/16-color where applicable,
transparent index 0 for sprites, and fixed frame dimensions.

## SNES implementation

Toolchain:

- PVSnesLib 4.6.0
- 816-tcc
- wla-65816 / wlalink
- gfx4snes
- devkitPro MSYS2
- Mesen2 for PC testing

Video:

- Mode 1
- 256×224
- BG0 = gameplay/title/intro artwork
- BG1 is reserved for future HUD/text work and is currently not initialized
  by the game state.

### VRAM layout

- `0x0000–0x37FF` — dynamic large OBJ pool
- `0x3800–0x3BFF` — dynamic small OBJ pool
- `0x4000–0x77FF` — BG0 tile data
- `0x7800–0x7FFF` — BG0 64×32 tilemap

BG character bases must remain 4K-word aligned. Do not invent arbitrary BG
tile addresses.

The reference-derived gameplay tileset currently uses about 27.8 KB of tile
data, fitting inside the reserved BG0 tile region.

## Gameplay

Controls:

- D-pad — move
- A — jump
- B — Crescent Slash
- START — advance/skip title and intro

Current engine systems:

- game state
- input
- horizontal camera
- tile collision
- player physics/animation
- combat
- scarab
- snake
- tiny mummy
- desert rat
- Golden Scorpion boss

## Important development rule

Always test the actual ROM in Mesen2 after graphics/VRAM changes.

A clean compiler build does not prove that the PPU is rendering correctly.

When graphics fail, isolate:

1. screen/mode
2. BG tile base
3. BG map base
4. palette
5. VRAM size/overlap
6. OAM
7. actual generated `.pic/.pal/.map` data

Do not rewrite the entire engine to solve a single rendering problem.

## Art priority

The supplied Forgotten Oasis reference is the visual target. Preserve its:

- character designs
- palette
- moonlit atmosphere
- Egyptian ruins
- pyramids
- palms
- grass
- water
- stonework
- silhouettes
- overall composition and pixel density

When adapting artwork to SNES limits, preserve the visual identity before adding
new gameplay features.
