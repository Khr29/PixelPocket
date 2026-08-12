#ifndef PLAYER_H
#define PLAYER_H

#include <snes.h>

// Loads BLAZE and places the player at the room's starting spot.
void player_init(void);

// Applies D-pad movement for one frame, clamped to the room's walls, and
// updates BLAZE's on-screen position.
void player_update(u16 padHeld);

u16 player_get_x(void);
u16 player_get_y(void);

#endif // PLAYER_H
