#include "level.h"

extern char level1tileset_til, level1tileset_tilend;
extern char level1tileset_pal, level1tileset_palend;

#define LEVEL_MAP_COLS 64
#define LEVEL_MAP_ROWS 32

static u16 mapbuf[LEVEL_MAP_COLS * LEVEL_MAP_ROWS];

void level_load(void) {
    u16 r, c;

    bgInitTileSet(
        LEVEL_BG, (u8 *)&level1tileset_til, (u8 *)&level1tileset_pal, LEVEL_BG_PAL_ROW,
        (u16)(&level1tileset_tilend - &level1tileset_til),
        (u16)(&level1tileset_palend - &level1tileset_pal),
        BG_16COLORS, LEVEL_BG_TILE_ADDR
    );

    for (r = 0; r < LEVEL_MAP_ROWS; r++) {
        for (c = 0; c < LEVEL_MAP_COLS; c++) {
            /* raw tile 0 is the sky meta-tile's top-left subtile (meta id 0,
               by construction in gen_level2.py) -- a safe filler for the
               map rows beyond LEVEL1_ROWS (SC_64x32 wants 32 rows; the
               level itself only defines 28, the rest scroll off-screen) */
            u8 tile = (r < LEVEL1_ROWS && c < LEVEL1_COLS) ? level1_tiles[r][c] : 0;
            mapbuf[(u16)r * LEVEL_MAP_COLS + c] = tile | BG_TIL_PAL(LEVEL_BG_PAL_ROW);
        }
    }

    bgInitMapSet(LEVEL_BG, (u8 *)mapbuf, sizeof(mapbuf), SC_64x32, LEVEL_BG_MAP_ADDR);
    bgSetEnable(LEVEL_BG);
    setScreenOn();
}

u8 level_tile_solid_at(s16 worldX, s16 worldY) {
    s16 col = worldX >> 3;
    s16 row = worldY >> 3;

    if (col < 0 || col >= LEVEL1_COLS) {
        return 1;
    }
    if (row < 0) {
        return 0;
    }
    if (row >= LEVEL1_ROWS) {
        return 1;
    }
    return tile_solid[level1_tiles[row][col]];
}
