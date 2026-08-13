#ifndef CHARACTER_H
#define CHARACTER_H

#include <snes.h>

// Roster sprite art. Each character is a single 32x32 OBJ (one OAM entry)
// with the actual ~16x32 chibi art left-aligned in that box -- the small
// SNES-RPG-protagonist scale from CLAUDE.md, not the original 64x64 art.
// BLAZE has a second "walk" frame (mid-stride legs) so walking can
// alternate between two frames instead of sliding.
//
// VRAM/CGRAM layout: each character gets its own non-overlapping OBJ tile
// address and OBJ palette bank so both can be shown at once.
//   BLAZE   -> OBJ tile word 0x0000, palette bank 0 (the player)
//   CAPTAIN -> OBJ tile word 0x0200, palette bank 1 (the town NPC)

// Uploads BLAZE's tiles/palette to VRAM/CGRAM. Call once at startup.
void blaze_load(void);
// Places (and shows) BLAZE with top-left corner at (x, y).
// walkFrame selects the mid-stride legs frame instead of the idle stance.
void blaze_place(s16 x, s16 y, bool walkFrame);

// Uploads CAPTAIN's tiles/palette to VRAM/CGRAM. Call once at startup.
void captain_load(void);
// Places (and shows) CAPTAIN with top-left corner at (x, y). Also resets
// the idle-bob animation's base position.
void captain_place(s16 x, s16 y);
// Advances CAPTAIN's subtle idle bob. Call once per frame.
void captain_update(void);

#endif // CHARACTER_H
