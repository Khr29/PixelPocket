#include "gfx.h"
#include "font5x7.h"

void gfx_rect(u16 *fb, int x, int y, int w, int h, u16 color) {
	int x0 = x < 0 ? 0 : x;
	int y0 = y < 0 ? 0 : y;
	int x1 = x + w > SCREEN_WIDTH ? SCREEN_WIDTH : x + w;
	int y1 = y + h > SCREEN_HEIGHT ? SCREEN_HEIGHT : y + h;

	for (int py = y0; py < y1; py++) {
		u16 *row = fb + py * SCREEN_WIDTH;
		for (int px = x0; px < x1; px++) {
			row[px] = color;
		}
	}
}

void gfx_char(u16 *fb, int x, int y, char c, u16 color, int scale) {
	const u8 *rows = font_glyph(c);

	for (int ry = 0; ry < FONT_H; ry++) {
		u8 bits = rows[ry];
		for (int rx = 0; rx < FONT_W; rx++) {
			if (bits & (1 << (FONT_W - 1 - rx))) {
				gfx_rect(fb, x + rx * scale, y + ry * scale, scale, scale, color);
			}
		}
	}
}

int gfx_text(u16 *fb, int x, int y, const char *s, u16 color, int scale) {
	int cx = x;
	int advance = (FONT_W + 1) * scale;

	for (const char *p = s; *p; p++) {
		gfx_char(fb, cx, y, *p, color, scale);
		cx += advance;
	}

	return cx - x - scale; // drop the trailing inter-char gap
}

int gfx_text_width(const char *s, int scale) {
	int len = 0;
	for (const char *p = s; *p; p++) len++;
	if (len == 0) return 0;
	return len * (FONT_W + 1) * scale - scale;
}
