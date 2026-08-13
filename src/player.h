#ifndef PLAYER_H
#define PLAYER_H

#include <snes.h>

#define PLAYER_W 16
#define PLAYER_H 32

// Loads BLAZE and places the player at the town's spawn spot.
void player_init(void);

// Applies D-pad movement for one frame (axis-separated so the player
// slides along walls instead of sticking), collides against town
// scenery, and advances the walk-leg animation. No-op while frozen
// (dialogue open).
void player_update(u16 padHeld, bool frozen);

s16 player_get_x(void);
s16 player_get_y(void);

#endif // PLAYER_H
