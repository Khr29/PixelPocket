#ifndef FONT5X7_H
#define FONT5X7_H

#include <gba_types.h>

// Minimal 5x7 pixel bitmap font. Only the glyphs needed by the welcome
// screen text are defined ("WELCOME KHALED", "PRESS START", "GAME STARTED").
// Each glyph is 7 rows; each row uses the low 5 bits (bit4=leftmost column).

#define FONT_W 5
#define FONT_H 7

// Returns a pointer to 7 bytes describing the glyph rows for c, or NULL
// if the character has no glyph (falls back to a blank space).
const u8 *font_glyph(char c);

#endif // FONT5X7_H
