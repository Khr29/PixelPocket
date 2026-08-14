#include "combat.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"

void combat_resolve(void) {
    s16 px, py, ex, ey, hx, hy;
    u8 pw, ph, ew, eh, hw, hh;
    u8 i;

    player_get_hurtbox(&px, &py, &pw, &ph);

    if (player_is_attack_active()) {
        player_get_attack_box(&hx, &hy, &hw, &hh);
        for (i = 0; i < ENEMY_COUNT; i++) {
            if (!enemy_is_active((EnemyId)i)) {
                continue;
            }
            enemy_get_hurtbox((EnemyId)i, &ex, &ey, &ew, &eh);
            if (collision_boxes_overlap(hx, hy, hw, hh, ex, ey, ew, eh)) {
                enemy_hurt((EnemyId)i, 1);
            }
        }
    }

    for (i = 0; i < ENEMY_COUNT; i++) {
        if (!enemy_is_dangerous((EnemyId)i)) {
            continue;
        }
        enemy_get_hurtbox((EnemyId)i, &ex, &ey, &ew, &eh);
        if (collision_boxes_overlap(px, py, pw, ph, ex, ey, ew, eh)) {
            player_take_damage(1, ex);
        }
    }
}
