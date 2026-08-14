#ifndef COLLISION_H
#define COLLISION_H

#include <snes.h>

// Moves a w x h box horizontally by dx (whole pixels, |dx| expected small --
// a fraction of a tile), stopping it flush against a solid tile if it would
// enter one. Samples the box's top and bottom edges (inset 1px) at the
// leading edge. Sets *hitWall if movement was clamped.
s16 collision_resolve_x(s16 x, s16 y, u8 w, u8 h, s16 dx, u8 *hitWall);

// Moves a w x h box vertically by dy, stopping it flush against a solid
// tile (ground when falling, ceiling when rising). Sets *onGround /
// *hitCeiling accordingly. Passing dy=0 still reports *onGround for the
// box's current resting position (used for coyote-less ground checks).
s16 collision_resolve_y(s16 x, s16 y, u8 w, u8 h, s16 dy, u8 *onGround, u8 *hitCeiling);

// True if two axis-aligned boxes overlap (used for hitbox/hurtbox tests).
u8 collision_boxes_overlap(s16 ax, s16 ay, u8 aw, u8 ah, s16 bx, s16 by, u8 bw, u8 bh);

#endif // COLLISION_H
