#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

//---------------------------------------------------------------------------------
// Minimal GBA test ROM: proves the compile -> link -> .gba pipeline works.
// Sets bitmap Mode 3 and fills the screen with a solid color. No gameplay yet.
//---------------------------------------------------------------------------------
int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	SetMode(MODE_3 | BG2_ENABLE);

	u16* framebuffer = (u16*)VRAM;
	u16 color = RGB5(8, 20, 31); // light blue

	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		framebuffer[i] = color;
	}

	while (1) {
		VBlankIntrWait();
	}
}
