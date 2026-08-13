/*---------------------------------------------------------------------------------
    DEDDY PARTY -- HOME/TOWN visual prototype (SNES)

    BLAZE walks around a small, handcrafted town square (D-pad, scenery
    collision), can talk to CAPTAIN with A, and can inspect the quest
    board with A. This is a visual/movement prototype only: no quest
    system, no AI, no additional areas yet (see CLAUDE.md).
---------------------------------------------------------------------------------*/
#include <snes.h>
#include "player.h"
#include "character.h"
#include "town.h"
#include "dialogue.h"

int main(void) {
    town_init();
    player_init();
    setScreenOn();

    while (1) {
        u16 pad;
        u16 padPressed;
        bool open = dialogue_is_open();

        WaitForVBlank();

        pad = padsCurrent(0);
        padPressed = padsDown(0);

        town_update_glow();
        player_update(pad, open);
        captain_update();
        oamUpdate();

        if (open) {
            if (padPressed & KEY_A) {
                dialogue_hide();
            }
        } else if (padPressed & KEY_A) {
            s16 px = player_get_x();
            s16 py = player_get_y();

            if (town_near_npc(px, py)) {
                dialogue_show(DIALOGUE_NPC);
            } else if (town_near_board(px, py)) {
                dialogue_show(DIALOGUE_BOARD);
            }
        }
    }

    return 0;
}
