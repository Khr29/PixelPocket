#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <snes.h>

// Sets up shared systems (text engine, sprite palettes) and shows the
// title screen. Call once at startup, before the main loop.
void game_state_init(void);

// Runs the current state's logic + draw for one frame, including any
// state transitions (TITLE -> INTRO -> GAMEPLAY). Call once per frame,
// after input_update().
void game_state_update(void);

#endif // GAME_STATE_H
