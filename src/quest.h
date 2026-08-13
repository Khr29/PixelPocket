#ifndef QUEST_H
#define QUEST_H

#include <snes.h>

typedef enum {
    STATE_DASHBOARD,
    STATE_QUEST_DETAILS,
    STATE_QUEST_COMPLETE,
    STATE_STATS,
} GameState;

// Sets up BG0/BG1, loads the font, and shows the dashboard. Call once at
// startup.
void quest_init(void);

// Applies one frame's newly-pressed buttons (padsDown-style bits) to the
// current state and re-renders if anything changed. Call once per frame.
void quest_update(u16 padPressed);

#endif // QUEST_H
