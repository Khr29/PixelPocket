#ifndef COMBAT_H
#define COMBAT_H

// Checks the player's crescent-slash hitbox against every enemy, and every
// dangerous enemy's hurtbox against the player, applying damage both ways.
// Call once per frame after player_update()/enemy_update_all().
void combat_resolve(void);

#endif // COMBAT_H
