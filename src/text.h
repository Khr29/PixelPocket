#ifndef TEXT_H
#define TEXT_H

#include <snes.h>

// Runtime bitmap-font text layer (BG1). The font tileset is the same 5x7
// glyph shapes used anywhere static text is still baked into chrome
// artwork, so dynamic and static text always match visually.
//
// Usage: text_clear() to reset the in-WRAM tilemap, any number of
// text_draw*() calls to stage text into it, then text_commit() once to
// push it all to VRAM in a single bgInitMapSet (screen briefly forces
// blank during that DMA -- see [[pvsneslib_bginit_blanks_screen]] --
// text_commit() calls setScreenOn() itself afterward).

#define TEXT_CREAM 1  // primary text color (palette row 1)
#define TEXT_GOLD 2   // accent color for key numbers (palette row 2)
#define TEXT_MUTED 3  // dimmed text -- completed quests, secondary info (palette row 3)
#define TEXT_TEAL 4   // section headers (palette row 4)
#define TEXT_GREEN 5  // success / completion (palette row 5)
#define TEXT_ORANGE 6 // streak (palette row 6)

// '#' (solid 8x8 block) used as a fill segment for bars/dots -- draw it
// with text_put(col,row,'#',color) like any other glyph.
// '^' (checkmark) similarly via text_put(col,row,'^',TEXT_GREEN).

// Loads the font tileset + both color palettes onto BG1. Call once at
// startup, before the first text_commit().
void text_init(void);

// Resets the staged (WRAM) tilemap to all-blank.
void text_clear(void);

// Stages one character at tile column/row (0-31, 0-27).
void text_put(u8 col, u8 row, char c, u8 color);

// Stages a null-terminated string starting at column/row, left to right.
// Returns the column just past the last character.
u8 text_draw(u8 col, u8 row, const char *str, u8 color);

// Stages an unsigned 16-bit value in plain decimal (no leading zeros,
// "0" prints as "0"). Returns the column just past the last digit.
u8 text_draw_u16(u8 col, u8 row, u16 value, u8 color);

// Same as text_draw_u16 but with a thousands separator, e.g. 12,480.
u8 text_draw_u16_comma(u8 col, u8 row, u16 value, u8 color);

// Digit count of value in plain decimal (no comma), e.g. 480 -> 3.
u8 text_u16_width(u16 value);

// Stages a `cells`-wide bar at column/row: floor(cells*num/den) solid
// filled cells in `color`, the rest left blank (showing the chrome's own
// baked bar track through the transparent remainder).
void text_draw_bar(u8 col, u8 row, u8 cells, u16 num, u16 den, u8 color);

// Stages `count` small square dots, `filled` of them solid (in `color`),
// the rest left blank.
void text_draw_dots(u8 col, u8 row, u8 count, u8 filled, u8 color);

// Pushes the staged tilemap to VRAM and turns the screen back on.
void text_commit(void);

#endif // TEXT_H
