#ifndef PARTY_H
#define PARTY_H

#include <snes.h>

// Playable floor area, inside the room's wall border (see
// assets/backgrounds/party_room/room.png).
#define ROOM_LEFT 8
#define ROOM_TOP 8
#define ROOM_RIGHT 248
#define ROOM_BOTTOM 216

// CAPTAIN's fixed spot in the room (top-left of his 64x64 sprite).
#define NPC_X 152
#define NPC_Y 88

// Loads the room background and CAPTAIN, and switches into BG_MODE1 with
// the room visible. Call once at startup, before player_init().
void party_init(void);

// True once per approach: the player is next to CAPTAIN and just pressed A.
bool party_check_interaction(u16 playerX, u16 playerY, bool interactPressed);

#endif // PARTY_H
