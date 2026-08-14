#include "collision.h"
#include "level.h"

s16 collision_resolve_x(s16 x, s16 y, u8 w, u8 h, s16 dx, u8 *hitWall) {
    s16 nx = (s16)(x + dx);
    *hitWall = 0;

    if (dx > 0) {
        if (level_tile_solid_at((s16)(nx + w - 1), (s16)(y + 1)) ||
            level_tile_solid_at((s16)(nx + w - 1), (s16)(y + h - 2))) {
            nx = (s16)((((nx + w - 1) >> 3) << 3) - w);
            *hitWall = 1;
        }
    } else if (dx < 0) {
        if (level_tile_solid_at(nx, (s16)(y + 1)) ||
            level_tile_solid_at(nx, (s16)(y + h - 2))) {
            nx = (s16)(((nx >> 3) + 1) << 3);
            *hitWall = 1;
        }
    }

    return nx;
}

s16 collision_resolve_y(s16 x, s16 y, u8 w, u8 h, s16 dy, u8 *onGround, u8 *hitCeiling) {
    s16 ny = (s16)(y + dy);
    *onGround = 0;
    *hitCeiling = 0;

    if (dy > 0) {
        if (level_tile_solid_at((s16)(x + 1), (s16)(ny + h - 1)) ||
            level_tile_solid_at((s16)(x + w - 2), (s16)(ny + h - 1))) {
            ny = (s16)((((ny + h - 1) >> 3) << 3) - h);
            *onGround = 1;
        }
    } else if (dy < 0) {
        if (level_tile_solid_at((s16)(x + 1), ny) ||
            level_tile_solid_at((s16)(x + w - 2), ny)) {
            ny = (s16)(((ny >> 3) + 1) << 3);
            *hitCeiling = 1;
        }
    } else {
        if (level_tile_solid_at((s16)(x + 1), (s16)(y + h)) ||
            level_tile_solid_at((s16)(x + w - 2), (s16)(y + h))) {
            *onGround = 1;
        }
    }

    return ny;
}

u8 collision_boxes_overlap(s16 ax, s16 ay, u8 aw, u8 ah, s16 bx, s16 by, u8 bw, u8 bh) {
    if (ax + aw <= bx || bx + bw <= ax) {
        return 0;
    }
    if (ay + ah <= by || by + bh <= ay) {
        return 0;
    }
    return 1;
}
