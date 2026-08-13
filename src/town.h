#ifndef TOWN_H
#define TOWN_H

#include <snes.h>

// Screen/world bounds -- this prototype is a single non-scrolling screen.
#define WORLD_W 256
#define WORLD_H 224

// Player's spawn spot, just below the house door on the path.
#define PLAYER_SPAWN_X 36
#define PLAYER_SPAWN_Y 66

// CAPTAIN's fixed spot, standing in the gap between the sign and the
// quest board.
#define NPC_X 112
#define NPC_Y 95

// Quest board hotspot (matches the drawn board in town.png).
#define BOARD_X 128
#define BOARD_Y 80
#define BOARD_W 32
#define BOARD_H 32

// Loads the town background (BG0) and places CAPTAIN. Call once at
// startup, before player_init().
void town_init(void);

// Advances the quest board's palette-pulse glow. Call once per frame.
void town_update_glow(void);

// True if the wxh box at (x,y) overlaps solid scenery (house, trees,
// fence, board, sign, hedges, crate/barrel, or CAPTAIN).
bool town_collides(s16 x, s16 y, u8 w, u8 h);

// True if (x,y) (player's top-left) is close enough to the quest board
// to inspect it.
bool town_near_board(s16 x, s16 y);

// True if (x,y) (player's top-left) is close enough to CAPTAIN to talk.
bool town_near_npc(s16 x, s16 y);

#endif // TOWN_H
