#include <snes.h>
#include "player.h"
#include "character.h"
#include "town.h"

#define PLAYER_SPEED 1
#define WALK_ANIM_FRAMES 8

static s16 playerX;
static s16 playerY;
static u8 animTimer;
static bool walkFrame;

void player_init(void) {
    blaze_load();

    playerX = PLAYER_SPAWN_X;
    playerY = PLAYER_SPAWN_Y;
    animTimer = 0;
    walkFrame = false;

    blaze_place(playerX, playerY, false);
}

void player_update(u16 padHeld, bool frozen) {
    s8 dx = 0, dy = 0;
    bool moving;

    if (!frozen) {
        if (padHeld & KEY_LEFT) dx = -1;
        else if (padHeld & KEY_RIGHT) dx = 1;
        if (padHeld & KEY_UP) dy = -1;
        else if (padHeld & KEY_DOWN) dy = 1;
    }
    moving = (dx != 0) || (dy != 0);

    if (dx != 0) {
        s16 nx = playerX + dx * PLAYER_SPEED;
        if (!town_collides(nx, playerY, PLAYER_W, PLAYER_H)) playerX = nx;
    }
    if (dy != 0) {
        s16 ny = playerY + dy * PLAYER_SPEED;
        if (!town_collides(playerX, ny, PLAYER_W, PLAYER_H)) playerY = ny;
    }

    if (moving) {
        animTimer++;
        if (animTimer >= WALK_ANIM_FRAMES) {
            animTimer = 0;
            walkFrame = !walkFrame;
        }
    } else {
        animTimer = 0;
        walkFrame = false;
    }

    blaze_place(playerX, playerY, walkFrame);
}

s16 player_get_x(void) { return playerX; }
s16 player_get_y(void) { return playerY; }
