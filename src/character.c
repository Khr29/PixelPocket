#include <snes.h>
#include "character.h"
#include "../assets/characters/blaze/blaze.inc"
#include "../assets/characters/captain/captain.inc"

// The SNES has one shared OBJSEL register selecting the OBJ tile name
// base, so only the FIRST sprite is loaded via oamInitGfxSet (which sets
// it up). Every other sprite is uploaded directly with dmaCopyVram/
// dmaCopyCGram at an address relative to that same base, per the pattern
// pvsneslib's own multi-sprite examples use (see snes-examples/graphics/
// Sprites/ObjectSize).

#define BLAZE_OAM_ID 0
#define BLAZE_PALETTE_ENTRY 0
#define BLAZE_TILE_ADDR 0x0000

#define CAPTAIN_OAM_ID 1
#define CAPTAIN_PALETTE_ENTRY 1
#define CAPTAIN_TILE_ADDR 0x0400 // past BLAZE's 64 tiles (64 tiles * 16 words/tile)

void blaze_load(void) {
    oamInitGfxSet(
        &blaze_til, (u16)(&blaze_tilend - &blaze_til),
        &blaze_pal, (u16)(&blaze_palend - &blaze_pal),
        BLAZE_PALETTE_ENTRY, BLAZE_TILE_ADDR, OBJ_SIZE32_L64
    );
}

void blaze_place(u16 x, u16 y) {
    oamSet(BLAZE_OAM_ID, x, y, 3, 0, 0, BLAZE_TILE_ADDR / 16, BLAZE_PALETTE_ENTRY);
    oamSetEx(BLAZE_OAM_ID, OBJ_LARGE, OBJ_SHOW);
}

void captain_load(void) {
    dmaCopyVram((u8 *)&captain_til, CAPTAIN_TILE_ADDR, (u16)(&captain_tilend - &captain_til));
    dmaCopyCGram((u8 *)&captain_pal, 128 + CAPTAIN_PALETTE_ENTRY * 16, (u16)(&captain_palend - &captain_pal));
}

void captain_place(u16 x, u16 y) {
    oamSet(CAPTAIN_OAM_ID, x, y, 2, 0, 0, CAPTAIN_TILE_ADDR / 16, CAPTAIN_PALETTE_ENTRY);
    oamSetEx(CAPTAIN_OAM_ID, OBJ_LARGE, OBJ_SHOW);
}

void characters_hide_all(void) {
    oamSetVisible(BLAZE_OAM_ID, OBJ_HIDE);
    oamSetVisible(CAPTAIN_OAM_ID, OBJ_HIDE);
}
