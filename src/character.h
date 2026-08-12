#ifndef CHARACTER_H
#define CHARACTER_H

#include <snes.h>

// Trump parody character ("MR. T.") -- one 64x64 OBJ sprite, native SNES
// 4bpp tile/palette data converted from assets/characters/trump/trump.png
// by gfx4snes at build time (see data.asm and the Makefile's trump.pic rule).

// Uploads the sprite's tiles and palette to VRAM/CGRAM. Call once at startup.
void trump_load(void);

// Places (and shows) the sprite with its top-left corner at (x, y).
void trump_place(u16 x, u16 y);

#endif // CHARACTER_H
