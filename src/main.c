/*---------------------------------------------------------------------------------
    DEDDY PARTY -- first playable prototype (SNES)

    BLAZE walks around a small checkered party room (D-pad, wall collision)
    and can walk up to CAPTAIN CLUTCH and press A to complete the party.
    No title screen, no character select, no menus/scoring yet -- just the
    smallest possible playable loop: move, interact, win.
---------------------------------------------------------------------------------*/
#include <snes.h>
#include "player.h"
#include "party.h"
#include "character.h"

#define BACKDROP_COLOR RGB5(9, 11, 16)

static void show_win_screen(void) {
    characters_hide_all();

    consoleInitDefaultText(0);
    bgSetGfxPtr(0, 0x3000);
    bgSetMapPtr(0, 0x6800, SC_32x32);
    setPaletteColor(0, BACKDROP_COLOR);

    consoleDrawText(7, 11, "PARTY COMPLETE!");
    consoleDrawText(2, 14, "You found Captain Clutch");
    consoleDrawText(4, 17, "and started the party.");
    consoleDrawText(4, 21, "Press B to play again");
}

int main(void) {
    while (1) {
        party_init();
        player_init();
        setScreenOn();

        while (1) {
            WaitForVBlank();

            u16 pad = padsCurrent(0);
            player_update(pad);

            if (party_check_interaction(player_get_x(), player_get_y(), (padsDown(0) & KEY_A) != 0)) {
                break;
            }
        }

        show_win_screen();

        while (1) {
            WaitForVBlank();
            if (padsDown(0) & KEY_B) {
                break;
            }
        }
    }

    return 0;
}
