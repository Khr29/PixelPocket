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
    // Mode 1: BG0 (our level+title+intro layer) is 16-color and the only
    // background actually initialized right now. BG1 is reserved for future
    // HUD/text (see level.h) but has no tile/map data loaded yet, and BG2/3
    // are unused entirely -- leaving any of them enabled makes the PPU fetch
    // tiles from whatever VRAM their registers happen to point at (e.g. the
    // OAM sprite pool at 0x0000), which shows up as garbled sprite-sized
    // noise over the picture. Every PVSnesLib example also calls setMode()
    // before the first setScreenOn() -- omitting it leaves REG_BGMODE at its
    // hardware-reset value, so the PPU never learns our tile data is
    // 4bpp/16-color and every BG tile fetch comes out corrupted.
    setMode(BG_MODE1, 0);
    bgSetDisable(1);
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
