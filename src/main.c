/*---------------------------------------------------------------------------------
    YAQUB: THE SLEEPY GUARDIANS OF THE NILE (SNES)

    A compact 2D pixel-art action platformer -- no world map, no character
    select, just Yaqub, the Moon Claw, and a very sleepy sense of adventure.

    D-PAD: move   A: jump   B: crescent-slash attack   START: pause/skip
---------------------------------------------------------------------------------*/
#include <snes.h>
#include "input.h"
#include "game_state.h"

int main(void) {
    // Mode 1: BG1 (our BG0/level+title+intro layer) and BG2 (our BG1/text
    // layer) are both 16-color; BG3 is unused. Every PVSnesLib example calls
    // this before the first setScreenOn() -- omitting it leaves REG_BGMODE
    // at its hardware-reset value, so the PPU never learns our tile data is
    // 4bpp/16-color and every BG tile fetch comes out corrupted.
    setMode(BG_MODE1, 0);
    bgSetDisable(2);
    bgSetDisable(3);

    // Dynamic OAM sprite engine: large-sprite pool (player/enemies/boss
    // quadrants, all drawn 32x32 via oamDynamic32Draw + OBJ_SIZEL) at
    // 0x0000, small-sprite pool (crescent slash, 16x16 via oamDynamic16Draw
    // + OBJ_SIZES) at 0x3800. See level.h for the full VRAM budget.
    oamInitDynamicSprite(0x0000, 0x3800, 0, 0, OBJ_SIZE16_L32);

    game_state_init();
    setScreenOn();

    while (1) {
        WaitForVBlank();
        input_update();
        game_state_update();
        oamInitDynamicSpriteEndFrame();
        oamVramQueueUpdate();
    }

    return 0;
}
