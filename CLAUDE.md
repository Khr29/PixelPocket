==================================================
PROJECT IDENTITY -- READ THIS FIRST
==================================================

This project is YAQUB: THE SLEEPY GUARDIANS OF THE NILE, a 2D pixel-art
action platformer in the Game Boy / Game Boy Color tradition, running on
native SNES hardware via PVSnesLib and targeting the Anbernic RG35XX
Plus's SNES emulator core.

It replaced an earlier project in this same repo/directory (QUEST
DASHBOARD -- a non-game daily-quest progression UI, and before that an
RPG-town prototype called "Deddy Party"). Both are gone; do not resurrect
dashboard/XP/streak/quest-tracker concepts or town/NPC-walking concepts
here unless the user explicitly asks for a new project again.

The game is explicitly NOT:

- a productivity dashboard or information screen
- an RPG town/house/explorable overworld with idle NPCs to talk to
- a game with character-select or a large ensemble cast on screen at once

It IS a compact side-scrolling action platformer: explore, fight small
enemies with a short claw attack, collect treasure, find secrets, beat a
boss. Camera is a classic 2D side-scroller (player kept near center,
tile-based horizontal scrolling).

==================================================
STORY (for flavor/reference -- do not overbuild beyond Level 1)
==================================================

The Kingdom of Qamar was protected by six legendary but chronically
sleepy feline guardians. YAQUB QAMAR AD-DIN DIBIAZAH -- a Bengal-marked
warrior cat with the Moon Claw -- is the only one playable right now. An
earthquake cracks open the desert near his sleeping spot outside a ruined
temple; a scarab lands on his face; he chases it, and that's the
tutorial. The antagonist (not yet implemented) is THE SAND PHARAOH. The
tone: the world is drawn seriously/moodily, the characters are
ridiculous. Humor is short -- one line at a time, not every screen.

Do NOT implement: character switching / the other 5 guardians, the Sand
Pharaoh boss, levels 2-6, inventory, save system, procedural generation,
or multiplayer/online features unless explicitly asked. Level 1 (THE
FORGOTTEN OASIS) is the whole scope for now.

==================================================
VISUAL STYLE
==================================================

Chunky, hard-edged, low-color pixel art -- Game Boy Color-era action
platformer, not modern vector/gradient/anti-aliased art. Dark nighttime
palette (deep indigo/navy sky), moonlight, warm sand, weathered stone,
restrained accent colors (gold for the Moon Claw and important UI, warm
cream text). No blurry scaling, no photographic textures, no realistic
3D, no modern rounded-card UI.

Sprites are authored at native pixel size: 32x32 for Yaqub, the scarab,
snake, tiny mummy, and desert rat; the Golden Scorpion boss is drawn as
four 32x32 quadrants composited into one 64x64 creature (see "SNES
IMPLEMENTATION NOTES" -- the dynamic OAM engine doesn't expose a real
64x64 draw call). Environment tiles are authored at 16x16 but converted
and stored as four 8x8 engine tiles each (again see below) -- never drawn
large and shrunk.

==================================================
CURRENT ART PIPELINE
==================================================

There is no dedicated pixel-art tool in this environment. Current art
(assets/player, assets/enemies, assets/levels, assets/ui/title,
assets/ui/intro) was generated programmatically with a Pillow (Python)
script using simple geometric primitives at native resolution, then
quantized to an explicit <=16-color indexed palette per asset (index 0 =
transparent for sprites) so gfx4snes can convert it. A real reference art
bible (user-supplied) drives the palette, Yaqub's design (Bengal spots,
green collar), and the tileset/scene content, but the reference image
itself is an AI-generated continuous-tone illustration (210k+ distinct
colors, soft shading/anti-aliasing) -- NOT real pixel art -- so it was
used as a direction reference, not extracted from directly; cropping and
downsampling it was tried and confirmed to produce illegible, muddy
sprites, incompatible with SNES 4bpp tiles and this project's own
no-anti-aliasing rule. Current art is still placeholder-quality (readable
and on-style, not hand-crafted final art). If the user provides real
*pixel* art, replace the PNGs in place; the Makefile/gfx4snes/data.asm
pipeline doesn't need to change as long as pixel dimensions, frame-grid
layout, and <=16-color-per-asset stay the same (see comments in
src/player.h, src/enemy.h, and the Makefile's bitmaps rules for each
asset's exact frame grid).

==================================================
SNES IMPLEMENTATION NOTES
==================================================

Toolchain: PVSnesLib (816-tcc / wla-65816 / wlalink / gfx4snes), installed
to C:/snesdev, built via devkitPro's bundled MSYS2 bash -- see README.md.
This was chosen over an actual Game Boy Color toolchain (GBDK-2020/RGBDS)
because this machine's dev setup is PVSnesLib-only; the RG35XX Plus runs
SNES natively too, so the proven build->run->test loop was kept and the
*art direction* targets the GBC feel instead of the GBC hardware.

Engine architecture (src/): game_state (TITLE/INTRO/GAMEPLAY), input,
camera, level (tilemap + per-tile collision data), collision (AABB vs.
tile grid), player (physics/animation/attack), enemy (scarab/snake/mummy/
rat/boss AI), combat (hitboxes, health), text (reused from the old
project -- generic BG1 bitmap-font renderer, used for HUD + intro
dialogue).

This is a hand-rolled lightweight engine using PVSnesLib's low-level BG
primitives directly (bgInitTileSet/bgInitMapSet/bgSetScroll) for
backgrounds, and its *dynamic OAM sprite engine* (oamInitDynamicSprite +
oambuffer[]/oamDynamic16Draw/oamDynamic32Draw) for sprites -- NOT raw
oamSet (its gfxoffset expects literal hardware tile numbers with a
128px-VRAM-row wraparound quirk that isn't worth the risk) and NOT
PVSnesLib's higher-level object/map engine (objInitEngine/mapLoad/
tmx2snes, which expects levels authored in Tiled .tmj with per-tile
custom properties -- not practical to hand-write reliably outside the
Tiled editor). The level's tile grid + collision table lives directly as
a C array in src/level1_data.h.

Two dimension-mapping quirks worth knowing before touching this again:
- The dynamic sprite engine has no real 64x64 draw call, so the Golden
  Scorpion is four 32x32 "quadrant" sprites (their own OAM slots) drawn
  together each frame -- see enemy.c's boss_quadrant_frames()/draw_boss().
  Mirroring it for the opposite facing direction requires swapping which
  quadrant-graphic renders on which side, not just setting the hflip bit.
- Environment tiles are authored at 16x16 in assets/levels/level1_tileset.png
  but converted at gfx4snes -s8 (8x8), the same proven flag used for every
  other BG asset -- adopting the PPU's separate 16x16-BG-tile hardware
  mode (a real but much less common/documented path) was judged not worth
  the risk. Each 16x16 authored tile is placed as a 2x2 block of matching
  8x8 engine cells; level1_data.h's raw tile indices already account for
  this (see the meta_subtiles() formula in scratchpad/gen_level2.py, the
  script that generated it).

VRAM layout is fully documented with the actual measured .pic byte sizes
in src/level.h's header comment -- read that before changing any VRAM
address in this project. Do not estimate sprite/tile byte sizes by eye;
gfx4snes pads any sprite sheet narrower than 128px up to 128px, so a
64x32 2-frame enemy sheet costs 4 frames' worth of VRAM, not 2. Check the
actual generated .pic file size after a build instead.

Gotchas hit while building this project (see this repo's Claude memory
for full detail if debugging OAM/BG issues again):
- `setMode(BG_MODE1, ...)` MUST be called before the first `setScreenOn()`
  -- every PVSnesLib example does this and it's easy to forget; omitting
  it leaves REG_BGMODE at its hardware-reset value, so the PPU doesn't
  know your tile data is 4bpp/16-color and every BG tile fetch comes out
  as repeating, doubled, corrupted-looking garbage.
- The dynamic sprite engine's `oambuffer[].oamattribute` bit0 is
  repurposed from the static-sprite "tile number high bit" meaning to a
  size-select flag: OR in `OBJ_SIZEL` (0, large) or `OBJ_SIZES` (1, small)
  to match whichever of `oamDynamic16Draw`/`oamDynamic32Draw` you call for
  that sprite -- confirmed via pvsneslib's own DynamicEngineSprite.c
  example, which mixes both sizes through one OBJ_SIZE16_L32 pool.
- `bgInitTileSet`/`bgInitMapSet` force the screen blank as a VRAM-write
  safety measure and don't restore it; call `setScreenOn()` again
  afterward if they run after the game's initial `setScreenOn()`.
- `oamSet`/`oamSetEx`/`oamSetXY`'s `id` parameter is a byte offset into
  OAM (4 bytes/sprite), not a plain sprite index -- the Nth sprite's id is
  `N*4`. (Only relevant if raw oamSet is ever reintroduced; the dynamic
  engine's oambuffer[] is indexed as plain 0..127.)

==================================================
CONTROLS
==================================================

D-PAD: move   A: jump   B: crescent-slash attack   START: pause
(X/Y/SELECT reserved for future special ability / map / inventory --
not wired up yet, don't add unnecessary controls ahead of need)

==================================================
DO NOT OVERBUILD
==================================================

The current milestone is a single polished vertical slice: title screen
-> intro -> Level 1 (The Forgotten Oasis) -> Yaqub can move/jump/attack
-> scarab + snake + tiny mummy + desert rat enemies -> basic collision ->
health -> Golden Scorpion boss. Don't add more levels, more enemies, more
guardians, save data, or menus beyond this until asked.
