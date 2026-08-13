#include <snes.h>
#include "character.h"
#include "../assets/characters/blaze/blaze_idle.inc"
#include "../assets/characters/blaze/blaze_walk.inc"
#include "../assets/characters/captain/captain.inc"

#define BLAZE_OAM_ID 0
#define BLAZE_PALETTE_ENTRY 0
#define BLAZE_IDLE_ADDR 0x0000
#define BLAZE_WALK_ADDR 0x0100 // past BLAZE_IDLE's 32x32 = 16 subtiles = 0x100 words

#define CAPTAIN_OAM_ID 4 // oamSet/oamSetEx "id" is a byte offset into OAM (4 bytes/sprite), not a plain sprite index -- see snes-examples/graphics/Sprites/ObjectSize
#define CAPTAIN_PALETTE_ENTRY 1
#define CAPTAIN_ADDR 0x0200 // past BLAZE_WALK's 32x32

// Idle bob: a 1px vertical drift, slow enough to read as "breathing" not
// jitter -- CLAUDE.md asks for subtle animation, not attention-grabbing.
#define BOB_HALF_PERIOD_FRAMES 24

void blaze_load(void) {
    // First upload establishes the shared OBJSEL size pair (OBJ_SIZE32_L64,
    // i.e. large = 32x32) and BLAZE's palette; the walk frame reuses that
    // same palette, so it only needs its tile pixel data DMA'd in.
    oamInitGfxSet(
        &blaze_idle_til, (u16)(&blaze_idle_tilend - &blaze_idle_til),
        &blaze_idle_pal, (u16)(&blaze_idle_palend - &blaze_idle_pal),
        BLAZE_PALETTE_ENTRY, BLAZE_IDLE_ADDR, OBJ_SIZE32_L64
    );
    dmaCopyVram((u8 *)&blaze_walk_til, BLAZE_WALK_ADDR, (u16)(&blaze_walk_tilend - &blaze_walk_til));
}

void blaze_place(s16 x, s16 y, bool walkFrame) {
    u16 addr = walkFrame ? BLAZE_WALK_ADDR : BLAZE_IDLE_ADDR;

    oamSet(BLAZE_OAM_ID, (u16)x, (u16)y, 2, 0, 0, addr / 16, BLAZE_PALETTE_ENTRY);
    oamSetEx(BLAZE_OAM_ID, OBJ_SMALL, OBJ_SHOW);
}

void captain_load(void) {
    dmaCopyVram((u8 *)&captain_til, CAPTAIN_ADDR, (u16)(&captain_tilend - &captain_til));
    dmaCopyCGram((u8 *)&captain_pal, 128 + CAPTAIN_PALETTE_ENTRY * 16, (u16)(&captain_palend - &captain_pal));
}

static s16 captainBaseX, captainBaseY;
static u16 captainBobTimer;
static bool captainBobUp;

void captain_place(s16 x, s16 y) {
    captainBaseX = x;
    captainBaseY = y;
    captainBobTimer = 0;
    captainBobUp = false;

    oamSet(CAPTAIN_OAM_ID, (u16)x, (u16)y, 2, 0, 0, CAPTAIN_ADDR / 16, CAPTAIN_PALETTE_ENTRY);
    oamSetEx(CAPTAIN_OAM_ID, OBJ_SMALL, OBJ_SHOW);
}

void captain_update(void) {
    captainBobTimer++;
    if (captainBobTimer >= BOB_HALF_PERIOD_FRAMES) {
        captainBobTimer = 0;
        captainBobUp = !captainBobUp;
    }
    oamSetXY(CAPTAIN_OAM_ID, (u16)captainBaseX, (u16)(captainBobUp ? captainBaseY - 1 : captainBaseY));
}
