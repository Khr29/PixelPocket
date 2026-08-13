/*---------------------------------------------------------------------------------
    QUEST DASHBOARD (SNES)

    Retro daily-quest / personal-progression dashboard. No world, no
    character, no movement -- see CLAUDE.md for the full design direction.

    D-PAD UP/DOWN: move the quest cursor (dashboard only)
    A: select / confirm       B: back       START: stats
---------------------------------------------------------------------------------*/
#include <snes.h>
#include "quest.h"

int main(void) {
    quest_init();
    setScreenOn();

    while (1) {
        u16 padPressed;

        WaitForVBlank();
        padPressed = padsDown(0);

        quest_update(padPressed);
    }

    return 0;
}
