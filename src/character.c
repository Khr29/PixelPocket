#include <snes.h>
#include "character.h"
#include "../assets/characters/trump/trump.inc"

#define TRUMP_PALETTE_ENTRY 0

void trump_load(void) {
    oamInitGfxSet(
        &trump_til, (u16)(&trump_tilend - &trump_til),
        &trump_pal, (u16)(&trump_palend - &trump_pal),
        TRUMP_PALETTE_ENTRY, 0x0000, OBJ_SIZE32_L64
    );
}

void trump_place(u16 x, u16 y) {
    oamSet(0, x, y, 3, 0, 0, 0, TRUMP_PALETTE_ENTRY);
    oamSetEx(0, OBJ_LARGE, OBJ_SHOW);
}
