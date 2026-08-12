#ifndef CHARACTER_H
#define CHARACTER_H

#include <snes.h>

// Roster sprite art. Each character is one 64x64 OBJ sprite, native SNES
// 4bpp tile/palette data converted from its assets/characters/<name>/*.png
// by gfx4snes at build time (see data.asm and the Makefile's *.pic rules).
//
// VRAM/CGRAM layout: each character gets its own non-overlapping tile
// address and OBJ palette bank so more than one can be shown at once.
//   BLAZE   -> OBJ tile word 0x0000, palette bank 0 (the player)
//   CAPTAIN -> OBJ tile word 0x0400, palette bank 1 (the party-room NPC)

// Uploads BLAZE's tiles/palette to VRAM/CGRAM. Call once at startup.
void blaze_load(void);
// Places (and shows) BLAZE at OAM slot 0, top-left corner at (x, y).
void blaze_place(u16 x, u16 y);

// Uploads CAPTAIN's tiles/palette to VRAM/CGRAM. Call once at startup.
void captain_load(void);
// Places (and shows) CAPTAIN at OAM slot 1, top-left corner at (x, y).
void captain_place(u16 x, u16 y);

// Hides both roster sprites (e.g. before switching to a text-only screen).
void characters_hide_all(void);

#endif // CHARACTER_H
