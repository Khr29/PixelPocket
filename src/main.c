/*---------------------------------------------------------------------------------
    DEDDY PARTY -- character art preview (SNES)

    Displays a single character -- the "MR. T." parody -- centered on a
    plain backdrop with its name underneath. No gameplay, no title screen,
    no other characters: this scene exists purely to evaluate the sprite
    art before anything else gets built on top of it.
---------------------------------------------------------------------------------*/
#include <snes.h>
#include "character.h"

#define BACKDROP_COLOR RGB5(9, 11, 16)

#define SPRITE_X 96
#define SPRITE_Y 56

//---------------------------------------------------------------------------------
int main(void) {
    // Text console on BG0 for the character's name (uses pvsneslib's built-in
    // default font -- no custom font asset needed for a two-line label).
    consoleInitDefaultText(0);
    bgSetGfxPtr(0, 0x3000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    trump_load();

    setMode(BG_MODE1, 0);
    bgSetDisable(1);
    bgSetDisable(2);

    setPaletteColor(0, BACKDROP_COLOR);

    consoleDrawText(13, 18, "MR. T.");
    consoleDrawText(4, 20, "(totally not a real person)");

    trump_place(SPRITE_X, SPRITE_Y);

    setScreenOn();

    while (1) {
        WaitForVBlank();
    }
    return 0;
}
