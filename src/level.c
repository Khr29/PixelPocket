#include "level.h"

extern char levelbg_til, levelbg_tilend;
extern char levelbg_pal, levelbg_palend;

void level_load(void) {
    bgInitTileSet(
        LEVEL_BG,
        (u8 *)&levelbg_til,
        (u8 *)&levelbg_pal,
        LEVEL_BG_PAL_ROW,
        (u16)(&levelbg_tilend - &levelbg_til),
        (u16)(&levelbg_palend - &levelbg_pal),
        BG_16COLORS,
        LEVEL_BG_TILE_ADDR
    );

    bgInitMapSet(
        LEVEL_BG,
        (u8 *)level_bg_map,
        sizeof(level_bg_map),
        SC_64x32,
        LEVEL_BG_MAP_ADDR
    );

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
