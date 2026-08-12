#include <snes.h>
#include "player.h"
#include "character.h"
#include "party.h"

#define PLAYER_SPEED 2
#define SPRITE_SIZE 64

static s16 playerX;
static s16 playerY;

void player_init(void) {
    blaze_load();

    playerX = ROOM_LEFT + 16;
    playerY = ROOM_BOTTOM - SPRITE_SIZE - 16;

    blaze_place((u16)playerX, (u16)playerY);
}

void player_update(u16 padHeld) {
    if (padHeld & KEY_LEFT) playerX -= PLAYER_SPEED;
    if (padHeld & KEY_RIGHT) playerX += PLAYER_SPEED;
    if (padHeld & KEY_UP) playerY -= PLAYER_SPEED;
    if (padHeld & KEY_DOWN) playerY += PLAYER_SPEED;

    if (playerX < ROOM_LEFT) playerX = ROOM_LEFT;
    if (playerY < ROOM_TOP) playerY = ROOM_TOP;
    if (playerX > ROOM_RIGHT - SPRITE_SIZE) playerX = ROOM_RIGHT - SPRITE_SIZE;
    if (playerY > ROOM_BOTTOM - SPRITE_SIZE) playerY = ROOM_BOTTOM - SPRITE_SIZE;

    blaze_place((u16)playerX, (u16)playerY);
}

u16 player_get_x(void) { return (u16)playerX; }
u16 player_get_y(void) { return (u16)playerY; }
