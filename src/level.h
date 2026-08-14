#ifndef LEVEL_H
#define LEVEL_H

#include <snes.h>
#include "level1_data.h"
#include "level_bg_map.h"

#define LEVEL1_PIXEL_WIDTH (LEVEL1_COLS * 8)
#define LEVEL1_PIXEL_HEIGHT (LEVEL1_ROWS * 8)

/*
 * SNES VRAM layout.
 *
 * 0x0000-0x37FF : dynamic large OBJ pool
 * 0x3800-0x3BFF : dynamic small OBJ pool (crescent slash)
 * 0x4000-0x77FF : BG0 Forgotten Oasis tiles (0x3800 words reserved)
 * 0x7800-0x7FFF : BG0 64x32 tilemap (0x800 words)
 *
 * The reference-derived gameplay tileset contains 896 unique 8x8 tiles
 * (28,672 bytes = 0x3800 words), exactly filling the BG0 tile slot. The
 * static opening characters are removed from the gameplay background so the
 * real Yaqub/enemy sprites can move over it; the 256px scene repeats
 * horizontally to provide a 512px playfield.
 */
#define LEVEL_BG 0
#define LEVEL_BG_TILE_ADDR 0x4000
#define LEVEL_BG_MAP_ADDR 0x7800
#define LEVEL_BG_PAL_ROW 0

/* Loads the reference-derived Forgotten Oasis artwork onto BG0. */
void level_load(void);

/* Point-solidity query in world pixel coordinates. */
u8 level_tile_solid_at(s16 worldX, s16 worldY);

#endif // LEVEL_H
