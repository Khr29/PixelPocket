#ifndef GFX_H
#define GFX_H

#include <gba_video.h>

// Small drawing helpers for Mode 3 (240x160, direct 15-bit color) bitmap
// rendering: pixel/rect fills and a bitmap-font text renderer.

static inline void gfx_pixel(u16 *fb, int x, int y, u16 color) {
	if ((unsigned)x < SCREEN_WIDTH && (unsigned)y < SCREEN_HEIGHT) {
		fb[y * SCREEN_WIDTH + x] = color;
	}
}

void gfx_rect(u16 *fb, int x, int y, int w, int h, u16 color);

// Draws one glyph scaled up by `scale`, top-left at (x, y).
void gfx_char(u16 *fb, int x, int y, char c, u16 color, int scale);

// Draws a string; returns the pixel width consumed.
int gfx_text(u16 *fb, int x, int y, const char *s, u16 color, int scale);

// Width in pixels a string would occupy at the given scale.
int gfx_text_width(const char *s, int scale);

#endif // GFX_H
