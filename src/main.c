#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>

#include "gfx.h"
#include "font5x7.h"

//---------------------------------------------------------------------------------
// Welcome / title screen for the RG35XX homebrew test ROM.
//
// Mode 3 (240x160, direct 15-bit color, single buffer). A dusk sky with
// silhouette mountains and a starfield sits behind the title, which slides
// down into place; a blinking PRESS START prompts the player. Pressing
// START swaps to a minimal second screen. All art is procedural (gradients,
// filled shapes, a hand-authored 5x7 bitmap font) so there are no external
// asset dependencies.
//---------------------------------------------------------------------------------

#define HORIZON_Y 100

#define TITLE_TEXT "WELCOME KHALED"
#define TITLE_SCALE 2
#define TITLE_FINAL_Y 40
#define TITLE_START_Y (-FONT_H * TITLE_SCALE)
#define TITLE_SLIDE_FRAMES 40

#define PROMPT_TEXT "PRESS START"
#define PROMPT_SCALE 2
#define PROMPT_Y 138
#define PROMPT_BLINK_FRAMES 20

#define STAR_COUNT 18
#define STAR_TWINKLE_PERIOD 20
#define STAR_TWINKLE_PHASES 4

typedef struct { u8 x, y; } Star;

static const Star STARS[STAR_COUNT] = {
	{ 10, 10 }, { 20, 30 }, { 8, 55 }, { 15, 70 }, { 24, 45 },
	{ 230, 12 }, { 220, 40 }, { 232, 60 }, { 224, 75 }, { 215, 25 },
	{ 60, 62 }, { 90, 70 }, { 120, 60 }, { 150, 75 }, { 170, 65 },
	{ 50, 80 }, { 180, 80 }, { 100, 82 },
};

typedef enum { SCENE_WELCOME, SCENE_GAME } Scene;

//---------------------------------------------------------------------------------
// Palette (filled in by main() since RGB5() needs to run at init time)
//---------------------------------------------------------------------------------
static u16 STAR_COLOR;
static u16 MOON_COLOR;
static u16 MTN_COLOR;
static u16 TITLE_COLOR;
static u16 TITLE_SHADOW;
static u16 PROMPT_COLOR;
static u16 GAME_BG_COLOR;
static u16 GAME_TEXT_COLOR;

//---------------------------------------------------------------------------------
// Background gradients (pure functions of y so any rectangle can be redrawn
// without needing an offscreen copy of the background).
//---------------------------------------------------------------------------------
static u16 sky_color(int y) {
	int r = 1 + (10 - 1) * y / (HORIZON_Y - 1);
	int g = 1 + (14 - 1) * y / (HORIZON_Y - 1);
	int b = 6 + (26 - 6) * y / (HORIZON_Y - 1);
	return RGB5(r, g, b);
}

static u16 ground_color(int y) {
	int local = y - HORIZON_Y;
	int span = SCREEN_HEIGHT - HORIZON_Y - 1;
	int r = 4 + (1 - 4) * local / span;
	int g = 18 + (8 - 18) * local / span;
	int b = 4 + (1 - 4) * local / span;
	return RGB5(r, g, b);
}

static u16 bg_color_at(int y) {
	return (y < HORIZON_Y) ? sky_color(y) : ground_color(y);
}

//---------------------------------------------------------------------------------
// Static decoration
//---------------------------------------------------------------------------------
static void draw_mountain(u16 *fb, int peakX, int peakY, int height, int halfWidthAtBase, u16 color) {
	for (int dy = 0; dy <= height; dy++) {
		int halfW = halfWidthAtBase * dy / height;
		gfx_rect(fb, peakX - halfW, peakY + dy, halfW * 2 + 1, 1, color);
	}
}

static void draw_moon(u16 *fb, int cx, int cy, int r) {
	for (int y = -r; y <= r; y++) {
		for (int x = -r; x <= r; x++) {
			if (x * x + y * y <= r * r) {
				gfx_pixel(fb, cx + x, cy + y, MOON_COLOR);
			}
		}
	}
}

static void draw_background(u16 *fb) {
	for (int y = 0; y < HORIZON_Y; y++) gfx_rect(fb, 0, y, SCREEN_WIDTH, 1, sky_color(y));
	for (int y = HORIZON_Y; y < SCREEN_HEIGHT; y++) gfx_rect(fb, 0, y, SCREEN_WIDTH, 1, ground_color(y));

	draw_mountain(fb, 120, 78, 22, 36, MTN_COLOR);
	draw_mountain(fb, 70, 82, 18, 30, MTN_COLOR);
	draw_mountain(fb, 170, 85, 15, 26, MTN_COLOR);

	draw_moon(fb, 218, 16, 7);

	for (int i = 0; i < STAR_COUNT; i++) {
		gfx_pixel(fb, STARS[i].x, STARS[i].y, STAR_COLOR);
	}
}

static void update_stars(u16 *fb, int frame) {
	int phase = frame / STAR_TWINKLE_PERIOD;
	for (int i = 0; i < STAR_COUNT; i++) {
		int on = ((phase + i) % STAR_TWINKLE_PHASES) != 0;
		u16 color = on ? STAR_COLOR : bg_color_at(STARS[i].y);
		gfx_pixel(fb, STARS[i].x, STARS[i].y, color);
	}
}

//---------------------------------------------------------------------------------
// Title slide-in
//---------------------------------------------------------------------------------
static void draw_title_at(u16 *fb, int x, int y) {
	gfx_text(fb, x + 1, y + 1, TITLE_TEXT, TITLE_SHADOW, TITLE_SCALE);
	gfx_text(fb, x, y, TITLE_TEXT, TITLE_COLOR, TITLE_SCALE);
}

static void erase_title_rect(u16 *fb, int x, int y, int w, int h) {
	for (int py = y; py < y + h; py++) {
		gfx_rect(fb, x, py, w, 1, bg_color_at(py));
	}
}

//---------------------------------------------------------------------------------
// Blinking prompt
//---------------------------------------------------------------------------------
static void set_prompt_visible(u16 *fb, int x, int y, int w, int h, int visible) {
	if (visible) {
		gfx_text(fb, x, y, PROMPT_TEXT, PROMPT_COLOR, PROMPT_SCALE);
	} else {
		erase_title_rect(fb, x, y, w, h);
	}
}

//---------------------------------------------------------------------------------
static void run_welcome_scene(u16 *fb) {
	draw_background(fb);

	int titleX = (SCREEN_WIDTH - gfx_text_width(TITLE_TEXT, TITLE_SCALE)) / 2;
	int titleW = gfx_text_width(TITLE_TEXT, TITLE_SCALE) + TITLE_SCALE; // include shadow overhang
	int titleH = FONT_H * TITLE_SCALE + TITLE_SCALE;

	int promptX = (SCREEN_WIDTH - gfx_text_width(PROMPT_TEXT, PROMPT_SCALE)) / 2;
	int promptW = gfx_text_width(PROMPT_TEXT, PROMPT_SCALE);
	int promptH = FONT_H * PROMPT_SCALE;

	int lastTitleY = TITLE_START_Y - 1000; // force first draw
	int lastPromptPhase = -1;
	int frame = 0;

	while (1) {
		VBlankIntrWait();
		scanKeys();
		frame++;

		if (keysDown() & KEY_START) {
			return;
		}

		update_stars(fb, frame);

		if (frame <= TITLE_SLIDE_FRAMES) {
			int y = TITLE_START_Y + (TITLE_FINAL_Y - TITLE_START_Y) * frame / TITLE_SLIDE_FRAMES;
			if (y != lastTitleY) {
				erase_title_rect(fb, titleX, lastTitleY, titleW, titleH);
				draw_title_at(fb, titleX, y);
				lastTitleY = y;
			}
		} else {
			int blinkFrame = frame - TITLE_SLIDE_FRAMES;
			int phase = (blinkFrame / PROMPT_BLINK_FRAMES) % 2;
			if (phase != lastPromptPhase) {
				set_prompt_visible(fb, promptX, PROMPT_Y, promptW, promptH, phase == 0);
				lastPromptPhase = phase;
			}
		}
	}
}

static void run_game_screen(u16 *fb) {
	gfx_rect(fb, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GAME_BG_COLOR);

	const char *text = "GAME STARTED";
	int x = (SCREEN_WIDTH - gfx_text_width(text, 2)) / 2;
	int y = (SCREEN_HEIGHT - FONT_H * 2) / 2;
	gfx_text(fb, x, y, text, GAME_TEXT_COLOR, 2);

	while (1) {
		VBlankIntrWait();
	}
}

int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	SetMode(MODE_3 | BG2_ENABLE);
	u16 *fb = (u16 *)VRAM;

	STAR_COLOR = RGB5(31, 31, 28);
	MOON_COLOR = RGB5(30, 29, 22);
	MTN_COLOR = RGB5(4, 4, 14);
	TITLE_COLOR = RGB5(31, 26, 4);
	TITLE_SHADOW = RGB5(5, 3, 0);
	PROMPT_COLOR = RGB5(31, 31, 31);
	GAME_BG_COLOR = RGB5(1, 1, 5);
	GAME_TEXT_COLOR = RGB5(10, 31, 29);

	Scene scene = SCENE_WELCOME;

	while (1) {
		if (scene == SCENE_WELCOME) {
			run_welcome_scene(fb);
			scene = SCENE_GAME;
		} else {
			run_game_screen(fb);
		}
	}
}
