#ifndef ENEMY_H
#define ENEMY_H

#include <snes.h>

typedef enum {
    ENEMY_SCARAB = 0,
    ENEMY_SNAKE = 1,
    ENEMY_MUMMY = 2,
    ENEMY_RAT = 3,
    ENEMY_BOSS = 4
} EnemyId;
#define ENEMY_COUNT 5

void enemy_init_level1(void);
void enemy_update_all(s16 playerX);
void enemy_draw_all(u16 camX);

// Parks every enemy/boss OAM slot off-screen without touching game state.
// enemy_draw_all() only runs during gameplay, so before that (title/intro)
// those hardware OAM entries are never written and can show stale/garbage
// tile data; call this once before the first frame that isn't gameplay.
void enemy_hide_all(void);

u8 enemy_is_active(EnemyId id);
void enemy_get_hurtbox(EnemyId id, s16 *x, s16 *y, u8 *w, u8 *h);

// False while the enemy can't currently hurt the player on contact (dead,
// mid hit-flash, or -- for the boss -- outside its brief attack windows).
u8 enemy_is_dangerous(EnemyId id);

// Applies 1 point of damage if the enemy is currently hittable (the boss
// only takes damage during its vulnerable window). No-op otherwise.
void enemy_hurt(EnemyId id, s8 amount);

u8 enemy_boss_defeated(void);

#endif // ENEMY_H
