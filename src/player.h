#ifndef PLAYER_H
#define PLAYER_H

#include <snes.h>

#define PLAYER_OAM_SLOT 0
#define PLAYER_SLASH_OAM_SLOT 1

#define PLAYER_SPRITE_W 32
#define PLAYER_SPRITE_H 32
#define PLAYER_MAX_HEARTS 3

// Frame indices into assets/player/yaqub.png's 4x4 grid of 32x32 cells
// (row-major, matches gfx4snes's -s 16 slicing order) -- exposed so
// game_state.c can drive a couple of scripted poses during the INTRO
// cutscene.
#define PLAYER_FRAME_IDLE0 0
#define PLAYER_FRAME_SLEEP0 13

void player_init(s16 x, s16 y);
void player_update(void);
void player_draw(u16 camX);

// Overrides the sprite's frame/facing directly (used by the INTRO state's
// scripted sleeping/waking beats, bypassing the normal physics FSM).
void player_set_pose(u8 frameId, u8 flipX);

s16 player_get_x(void);
s16 player_get_y(void);

// Tight hurtbox (inset from the full 32x32 sprite cell) used for both
// "was I hit" and "am I standing on the boss" style checks.
void player_get_hurtbox(s16 *x, s16 *y, u8 *w, u8 *h);

// Only meaningful while the crescent-slash is in its active window; returns
// 0 outside that window (callers should still check before using the box).
u8 player_is_attack_active(void);
void player_get_attack_box(s16 *x, s16 *y, u8 *w, u8 *h);

// Applies damage (if not currently invulnerable), sets knockback away from
// sourceX and a brief hurt/invuln window. No-op while already invulnerable
// or dead.
void player_take_damage(s8 amount, s16 sourceX);

u8 player_get_hearts(void);
u8 player_is_dead(void);

#endif // PLAYER_H
