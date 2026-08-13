#include <snes.h>
#include "text.h"

extern char font_til, font_tilend;
extern char font_pal, font_palend;

#define TEXT_BG 1
#define TEXT_TILE_ADDR 0x5000
#define TEXT_MAP_ADDR 0x7000
#define TEXT_MAP_COLS 32
#define TEXT_MAP_ROWS 32
#define TEXT_BLANK_TILE 36 // space glyph -- see FONT_CHARS order below
#define TEXT_PRIORITY_BIT (1 << 13) // force text above BG0's chrome art

// Must exactly match assets/ui/font's glyph order (see the generator
// script's FONT_CHARS) -- tile index == position in this string.
// "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .',/:+-!>%"
static u8 font_index(char c) {
    if (c >= 'A' && c <= 'Z') return (u8)(c - 'A');
    if (c >= 'a' && c <= 'z') return (u8)(c - 'a'); // treat lowercase as uppercase
    if (c >= '0' && c <= '9') return (u8)(26 + (c - '0'));
    switch (c) {
        case ' ': return 36;
        case '.': return 37;
        case ',': return 38;
        case '\'': return 39;
        case '/': return 40;
        case ':': return 41;
        case '+': return 42;
        case '-': return 43;
        case '!': return 44;
        case '>': return 45;
        case '%': return 46;
        case '^': return 47;
        case '#': return 48;
        default: return 36; // unknown characters render as blank
    }
}

static u16 mapbuf[TEXT_MAP_COLS * TEXT_MAP_ROWS];

// Hand-authored gold variant of the font's single ink color -- the tile
// pixel data (index 0 transparent / index 1 ink) is identical to the
// cream palette gfx4snes generated from font.png; only the CGRAM color
// index 1 maps to differs, letting one tileset serve two text colors by
// choosing a different palette row per glyph.
static const u16 GOLD_PALETTE[16] = {
    0, RGB5(29, 24, 9), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const u16 MUTED_PALETTE[16] = {
    0, RGB5(17, 18, 21), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const u16 TEAL_PALETTE[16] = {
    0, RGB5(10, 24, 24), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const u16 GREEN_PALETTE[16] = {
    0, RGB5(12, 24, 15), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const u16 ORANGE_PALETTE[16] = {
    0, RGB5(28, 17, 8), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void text_init(void) {
    bgInitTileSet(
        TEXT_BG, (u8 *)&font_til, (u8 *)&font_pal, TEXT_CREAM,
        (u16)(&font_tilend - &font_til), (u16)(&font_palend - &font_pal),
        BG_16COLORS, TEXT_TILE_ADDR
    );
    dmaCopyCGram((u8 *)GOLD_PALETTE, TEXT_GOLD * 16, 32);
    dmaCopyCGram((u8 *)MUTED_PALETTE, TEXT_MUTED * 16, 32);
    dmaCopyCGram((u8 *)TEAL_PALETTE, TEXT_TEAL * 16, 32);
    dmaCopyCGram((u8 *)GREEN_PALETTE, TEXT_GREEN * 16, 32);
    dmaCopyCGram((u8 *)ORANGE_PALETTE, TEXT_ORANGE * 16, 32);
    setScreenOn();
}

void text_clear(void) {
    u16 i;
    u16 blank = TEXT_BLANK_TILE | (TEXT_CREAM << 10) | TEXT_PRIORITY_BIT;
    for (i = 0; i < TEXT_MAP_COLS * TEXT_MAP_ROWS; i++) {
        mapbuf[i] = blank;
    }
}

void text_put(u8 col, u8 row, char c, u8 color) {
    if (col >= TEXT_MAP_COLS || row >= TEXT_MAP_ROWS) return;
    mapbuf[(u16)row * TEXT_MAP_COLS + col] = font_index(c) | (color << 10) | TEXT_PRIORITY_BIT;
}

u8 text_draw(u8 col, u8 row, const char *str, u8 color) {
    u8 c = col;
    while (*str) {
        text_put(c, row, *str, color);
        c++;
        str++;
    }
    return c;
}

static u8 utoa_digits(u16 value, char *out) {
    char tmp[6];
    u8 n = 0;
    u8 i;
    if (value == 0) {
        out[0] = '0';
        return 1;
    }
    while (value > 0 && n < 5) {
        tmp[n++] = (char)('0' + (value % 10));
        value /= 10;
    }
    for (i = 0; i < n; i++) {
        out[i] = tmp[n - 1 - i];
    }
    return n;
}

u8 text_u16_width(u16 value) {
    char digits[6];
    return utoa_digits(value, digits);
}

u8 text_draw_u16(u8 col, u8 row, u16 value, u8 color) {
    char digits[6];
    u8 n = utoa_digits(value, digits);
    u8 i;
    for (i = 0; i < n; i++) {
        text_put(col + i, row, digits[i], color);
    }
    return (u8)(col + n);
}

u8 text_draw_u16_comma(u8 col, u8 row, u16 value, u8 color) {
    char digits[6];
    u8 n = utoa_digits(value, digits);
    u8 c = col;
    u8 i;
    for (i = 0; i < n; i++) {
        u8 remaining = (u8)(n - i);
        text_put(c, row, digits[i], color);
        c++;
        if (remaining > 1 && (remaining - 1) % 3 == 0) {
            text_put(c, row, ',', color);
            c++;
        }
    }
    return c;
}

void text_draw_bar(u8 col, u8 row, u8 cells, u16 num, u16 den, u8 color) {
    u8 filled = (u8)(((u32)cells * num) / den);
    u8 i;
    for (i = 0; i < filled; i++) {
        text_put((u8)(col + i), row, '#', color);
    }
}

void text_draw_dots(u8 col, u8 row, u8 count, u8 filled, u8 color) {
    u8 i;
    for (i = 0; i < count; i++) {
        text_put((u8)(col + i), row, '#', i < filled ? color : TEXT_MUTED);
    }
}

void text_commit(void) {
    bgInitMapSet(TEXT_BG, (u8 *)mapbuf, sizeof(mapbuf), SC_32x32, TEXT_MAP_ADDR);
    bgSetEnable(TEXT_BG);
    setScreenOn();
}
