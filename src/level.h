#ifndef LEVEL_H
#define LEVEL_H

#include <snes.h>
#include "level1_data.h"

#define LEVEL1_PIXEL_WIDTH (LEVEL1_COLS * 8)
#define LEVEL1_PIXEL_HEIGHT (LEVEL1_ROWS * 8)

// BG0 tile/map VRAM slot. Shared with the title/intro full-screen pictures
// (see game_state.c) since they're never shown at the same time as the
// level -- reloading it per state is cheap and saves VRAM.
//
// Full VRAM budget (word addresses, see main.c's oamInitDynamicSprite call
// for the OAM pools; sizes below are the *measured* .pic byte counts from
// a clean build, not estimates -- gfx4snes pads any sprite sheet narrower
// than 128px up to 128px, so e.g. a 64x32 2-frame enemy sheet actually
// costs 4 frames' worth of VRAM):
//   0x0000-0x37FF  OAM large-sprite pool (player/enemies/boss quadrants,
//                  all 32x32 -- yaqub 8192B + 4x enemy 2048B each + boss
//                  8192B = 24576B = 0x3000 words exactly used, 0x0800
//                  words of margin reserved here)
//   0x3800-0x3BFF  OAM small-sprite pool (crescent slash, 16x16 -- 1024B
//                  = 0x200 words used)
//   0x3C00-0x5BFF  BG0 tile data (level1 tileset 3072B, reloaded with
//                  title.pic 9376B or intro.pic 5408B per game_state --
//                  0x2000 words reserved, comfortable margin over title's
//                  0x1250-word need)
//   0x5C00-0x63FF  BG0 tilemap (SC_64x32, 0x800 words, fixed size)
//   0x6400-0x6BFF  BG1 text tiles (see text.c; font.pic is 2048B)
//   0x6C00-0x6FFF  BG1 text tilemap (SC_32x32, 0x400 words, fixed size)
#define LEVEL_BG 0
#define LEVEL_BG_TILE_ADDR 0x3C00
#define LEVEL_BG_MAP_ADDR 0x5C00
#define LEVEL_BG_PAL_ROW 0

// Loads the level1 tileset + hand-authored tilemap onto BG0 and enables it.
void level_load(void);

// Point-solidity query in world pixel coordinates. Off the left/right/
// bottom edges counts as solid (acts as a wall/floor); above the top of
// the map counts as open sky.
u8 level_tile_solid_at(s16 worldX, s16 worldY);

#endif // LEVEL_H
