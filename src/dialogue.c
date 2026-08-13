#include <snes.h>
#include "dialogue.h"

extern char dlgnpc_til, dlgnpc_tilend;
extern char dlgnpc_map, dlgnpc_mapend;
extern char dlgnpc_pal, dlgnpc_palend;

extern char dlgboard_til, dlgboard_tilend;
extern char dlgboard_map, dlgboard_mapend;
extern char dlgboard_pal, dlgboard_palend;

#define DLG_BG 1
#define DLG_TILE_ADDR 0x5000
#define DLG_MAP_ADDR 0x7000
#define DLG_PALETTE_ENTRY 1

static bool isOpen = false;

void dialogue_show(DialogueKind kind) {
    if (kind == DIALOGUE_NPC) {
        bgInitTileSet(
            DLG_BG, (u8 *)&dlgnpc_til, (u8 *)&dlgnpc_pal, DLG_PALETTE_ENTRY,
            (u16)(&dlgnpc_tilend - &dlgnpc_til), (u16)(&dlgnpc_palend - &dlgnpc_pal),
            BG_16COLORS, DLG_TILE_ADDR
        );
        bgInitMapSet(DLG_BG, (u8 *)&dlgnpc_map, (u16)(&dlgnpc_mapend - &dlgnpc_map), SC_32x32, DLG_MAP_ADDR);
    } else {
        bgInitTileSet(
            DLG_BG, (u8 *)&dlgboard_til, (u8 *)&dlgboard_pal, DLG_PALETTE_ENTRY,
            (u16)(&dlgboard_tilend - &dlgboard_til), (u16)(&dlgboard_palend - &dlgboard_pal),
            BG_16COLORS, DLG_TILE_ADDR
        );
        bgInitMapSet(DLG_BG, (u8 *)&dlgboard_map, (u16)(&dlgboard_mapend - &dlgboard_map), SC_32x32, DLG_MAP_ADDR);
    }

    bgSetEnable(DLG_BG);
    // bgInitTileSet/bgInitMapSet force the screen blank as a safety measure
    // while writing VRAM outside of vblank; that's invisible when they run
    // once before the game's first setScreenOn(), but here they run
    // mid-game, so the display must be explicitly turned back on.
    setScreenOn();
    isOpen = true;
}

void dialogue_hide(void) {
    bgSetDisable(DLG_BG);
    isOpen = false;
}

bool dialogue_is_open(void) {
    return isOpen;
}
