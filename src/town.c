#include <snes.h>
#include "town.h"
#include "character.h"

// See the comment in party.c's predecessor for why these are hand-declared
// as char rather than the generated .inc's "unsigned short" typing --
// bgInitMapSet wants byte counts/pointers, and pointer subtraction on a
// u16* would silently halve the real size.
extern char town_til, town_tilend;
extern char town_map, town_mapend;
extern char town_pal, town_palend;

#define TOWN_BG_TILE_ADDR 0x1000
#define TOWN_BG_MAP_ADDR 0x4000
#define TOWN_PALETTE_ENTRY 0

// The quest board's accent-red star (palette color 14 of BG palette row 0)
// slowly pulses between these two RGB5 values -- a cheap, robust "small
// animated detail" that needs no extra VRAM tile bookkeeping.
#define GLOW_COLOR_A RGB5(27, 10, 10)
#define GLOW_COLOR_B RGB5(31, 18, 14)
#define GLOW_PERIOD_FRAMES 40

typedef struct {
    s16 x, y;
    u8 w, h;
} Rect;

// Solid scenery, matching assets/backgrounds/town/town.png's layout (see
// the generator's block grid: house cols1-4/rows0-3, big tree cols13-14/
// rows0-1, quest board cols8-9/rows5-6, sign col6/rows5-6, fence
// col5-6/row3, small trees, crate/barrel, and the top/bottom hedge rows).
static const Rect obstacles[] = {
    {16, 0, 64, 64},     // house
    {208, 0, 32, 32},    // big focal tree
    {BOARD_X, BOARD_Y, BOARD_W, BOARD_H},
    {96, 80, 16, 32},    // sign
    {80, 48, 32, 16},    // fence
    {176, 128, 16, 16},  // crate
    {192, 128, 16, 16},  // barrel
    {0, 0, 16, 16}, {0, 80, 16, 16}, {0, 144, 16, 16},       // small trees, left edge
    {240, 48, 16, 16}, {240, 144, 16, 16},                  // small trees, right edge
    {112, 160, 16, 16}, {192, 32, 16, 16}, {48, 160, 16, 16}, {144, 176, 16, 16}, {16, 192, 16, 16},
    {80, 0, 128, 16},    // top hedge row
    {0, 208, 256, 16},   // bottom hedge row
    {NPC_X, NPC_Y, 16, 32}, // CAPTAIN
};
#define OBSTACLE_COUNT (sizeof(obstacles) / sizeof(obstacles[0]))

static u16 glowTimer;
static bool glowHigh;

void town_init(void) {
    bgInitTileSet(
        0, (u8 *)&town_til, (u8 *)&town_pal, TOWN_PALETTE_ENTRY,
        (u16)(&town_tilend - &town_til), (u16)(&town_palend - &town_pal),
        BG_16COLORS, TOWN_BG_TILE_ADDR
    );
    bgInitMapSet(0, (u8 *)&town_map, (u16)(&town_mapend - &town_map), SC_32x32, TOWN_BG_MAP_ADDR);

    setMode(BG_MODE1, 0);
    bgSetDisable(1); // dialogue box starts closed
    bgSetDisable(2);

    captain_load();
    captain_place(NPC_X, NPC_Y);

    glowTimer = 0;
    glowHigh = false;
}

void town_update_glow(void) {
    glowTimer++;
    if (glowTimer >= GLOW_PERIOD_FRAMES) {
        glowTimer = 0;
        glowHigh = !glowHigh;
        setPaletteColor(14, glowHigh ? GLOW_COLOR_B : GLOW_COLOR_A);
    }
}

static bool rectsOverlap(s16 ax, s16 ay, u8 aw, u8 ah, s16 bx, s16 by, u8 bw, u8 bh) {
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

bool town_collides(s16 x, s16 y, u8 w, u8 h) {
    u8 i;
    if (x < 0 || y < 0 || x + w > WORLD_W || y + h > WORLD_H) {
        return true;
    }
    for (i = 0; i < OBSTACLE_COUNT; i++) {
        if (rectsOverlap(x, y, w, h, obstacles[i].x, obstacles[i].y, obstacles[i].w, obstacles[i].h)) {
            return true;
        }
    }
    return false;
}

bool town_near_board(s16 x, s16 y) {
    return rectsOverlap(x, y, 16, 32, BOARD_X - 12, BOARD_Y - 12, BOARD_W + 24, BOARD_H + 24);
}

bool town_near_npc(s16 x, s16 y) {
    return rectsOverlap(x, y, 16, 32, NPC_X - 12, NPC_Y - 12, 16 + 24, 32 + 24);
}
