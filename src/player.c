#include "player.h"
#include "input.h"
#include "collision.h"

extern char yaqub_til, yaqub_tilend;
extern char yaqub_pal, yaqub_palend;
extern char crescentslash_til, crescentslash_tilend;
extern char crescentslash_pal, crescentslash_palend;
extern char sleepz_til, sleepz_tilend;
extern char sleepz_pal, sleepz_palend;

#define PLAYER_PAL_ROW 0
#define SLASH_PAL_ROW 1
#define SLEEP_Z_PAL_ROW 7

// Frame indices into the yaqub.png 4x4 grid of 32x32 cells (row-major).
#define F_IDLE0 0
#define F_IDLE1 1
#define F_IDLE2 2
#define F_WALK0 3
#define F_WALK1 4
#define F_WALK2 5
#define F_WALK3 6
#define F_JUMP 7
#define F_FALL 8
#define F_ATTACK0 9
#define F_ATTACK1 10
#define F_ATTACK2 11
#define F_HURT 12
#define F_SLEEP 13

#define SLASH_F_CRESCENT 0
#define SLASH_F_BURST 1

#define WALK_SPEED 1
#define JUMP_VEL (-6)
#define MAX_FALL 5
#define GRAVITY_NUM 1
#define GRAVITY_DIV 4

#define ATTACK_FRAMES 18
#define ATTACK_ACTIVE_LO 2
#define ATTACK_ACTIVE_HI 8
#define HURT_FRAMES 24
#define INVULN_FRAMES 70
#define KNOCKBACK_X 2
#define KNOCKBACK_Y (-3)
#define IDLE_CYCLE_FRAMES 40

typedef enum { PSTATE_IDLE, PSTATE_WALK, PSTATE_JUMP, PSTATE_FALL, PSTATE_ATTACK, PSTATE_HURT } PlayerState;

static s16 px, py;
static s16 yvel, yvelAcc;
static u8 onGround;
static u8 facingLeft;
static PlayerState state;
static u8 stateTimer;
static u8 idleTimer;
static u8 animFrame;
static u8 invulnTimer;
static s8 hearts;
static u8 scriptedPose; // 1 while game_state.c is driving the sprite directly

static void refresh_anim(void) {
    switch (state) {
        case PSTATE_IDLE:
            idleTimer++;
            if (idleTimer >= IDLE_CYCLE_FRAMES * 3) {
                idleTimer = 0;
            }
            animFrame = (u8)(F_IDLE0 + (idleTimer / IDLE_CYCLE_FRAMES));
            break;
        case PSTATE_WALK:
            idleTimer = 0;
            animFrame = (u8)(F_WALK0 + ((stateTimer / 6) & 3));
            break;
        case PSTATE_JUMP:
            idleTimer = 0;
            animFrame = F_JUMP;
            break;
        case PSTATE_FALL:
            idleTimer = 0;
            animFrame = F_FALL;
            break;
        case PSTATE_ATTACK:
            idleTimer = 0;
            if (stateTimer > 12) {
                animFrame = F_ATTACK0;
            } else if (stateTimer > 6) {
                animFrame = F_ATTACK1;
            } else {
                animFrame = F_ATTACK2;
            }
            break;
        case PSTATE_HURT:
            idleTimer = 0;
            animFrame = F_HURT;
            break;
    }
    stateTimer++;
}

void player_init(s16 x, s16 y) {
    setPalette((u8 *)&yaqub_pal, 128 + PLAYER_PAL_ROW * 16, 16 * 2);
    setPalette((u8 *)&crescentslash_pal, 128 + SLASH_PAL_ROW * 16, 16 * 2);
    setPalette((u8 *)&sleepz_pal, 128 + SLEEP_Z_PAL_ROW * 16, 16 * 2);

    px = x;
    py = y;
    yvel = 0;
    yvelAcc = 0;
    onGround = 0;
    facingLeft = 0;
    state = PSTATE_IDLE;
    stateTimer = 0;
    idleTimer = 0;
    animFrame = F_IDLE0;
    invulnTimer = 0;
    hearts = PLAYER_MAX_HEARTS;
    scriptedPose = 0;

    oambuffer[PLAYER_OAM_SLOT].oamgraphics = (u8 *)&yaqub_til;
    oambuffer[PLAYER_OAM_SLOT].oamframeid = F_IDLE0;
    oambuffer[PLAYER_OAM_SLOT].oamattribute = OBJ_SIZEL | (2 << 4) | (PLAYER_PAL_ROW << 1);
    oambuffer[PLAYER_OAM_SLOT].oamrefresh = 1;

    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamgraphics = (u8 *)&sleepz_til;
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamframeid = 0;
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamattribute = OBJ_SIZES | (2 << 4) | (SLEEP_Z_PAL_ROW << 1);
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamrefresh = 1;
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamx = 255;
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamy = 240;

    oambuffer[PLAYER_SLASH_OAM_SLOT].oamgraphics = (u8 *)&crescentslash_til;
    oambuffer[PLAYER_SLASH_OAM_SLOT].oamframeid = SLASH_F_CRESCENT;
    oambuffer[PLAYER_SLASH_OAM_SLOT].oamattribute = OBJ_SIZES | (2 << 4) | (SLASH_PAL_ROW << 1);
    oambuffer[PLAYER_SLASH_OAM_SLOT].oamrefresh = 1;
    oambuffer[PLAYER_SLASH_OAM_SLOT].oamx = 255;
    oambuffer[PLAYER_SLASH_OAM_SLOT].oamy = 240;
}

void player_set_pose(u8 frameId, u8 flipX) {
    scriptedPose = 1;
    animFrame = frameId;
    facingLeft = flipX;
}

void player_set_position(s16 x, s16 y) {
    px = x;
    py = y;
}

void player_draw_sleep_z(s16 x, s16 y, u8 phase) {
    // The Zs are a separate 16x16 sprite so they can drift upward
    // independently of the sleeping cat. Movement is deliberately slow
    // and stepped to preserve the chunky handheld-pixel feel.
    s16 rise = (s16)((phase >> 4) & 7);
    s16 sx = (s16)(x + 18 - (rise >> 1));
    s16 sy = (s16)(y - 12 - rise);

    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamattribute =
        OBJ_SIZES | (2 << 4) | (SLEEP_Z_PAL_ROW << 1);
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamframeid = 0;
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamrefresh = 1;
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamx = (u16)sx;
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamy = (u16)sy;
    oamDynamic16Draw(PLAYER_SLEEP_Z_OAM_SLOT);
}

void player_update(void) {
    u16 held;
    u16 pressed;
    s16 dx = 0;
    u8 hitWall, hitCeil;

    scriptedPose = 0;

    if (invulnTimer > 0) {
        invulnTimer--;
    }

    held = input_held();
    pressed = input_pressed();

    if (state != PSTATE_ATTACK && state != PSTATE_HURT) {
        if (held & KEY_LEFT) {
            dx = -WALK_SPEED;
            facingLeft = 1;
        } else if (held & KEY_RIGHT) {
            dx = WALK_SPEED;
            facingLeft = 0;
        }
    }

    if (dx != 0) {
        px = collision_resolve_x(px, py, PLAYER_SPRITE_W, PLAYER_SPRITE_H, dx, &hitWall);
    }

    yvelAcc = (s16)(yvelAcc + GRAVITY_NUM);
    if (yvelAcc >= GRAVITY_DIV) {
        yvel++;
        yvelAcc = (s16)(yvelAcc - GRAVITY_DIV);
    }
    if (yvel > MAX_FALL) {
        yvel = MAX_FALL;
    }

    if ((pressed & KEY_A) && onGround && state != PSTATE_ATTACK && state != PSTATE_HURT) {
        yvel = JUMP_VEL;
        onGround = 0;
    }

    py = collision_resolve_y(px, py, PLAYER_SPRITE_W, PLAYER_SPRITE_H, yvel, &onGround, &hitCeil);
    if (onGround && yvel > 0) {
        yvel = 0;
    }
    if (hitCeil && yvel < 0) {
        yvel = 0;
    }

    if ((pressed & KEY_B) && state != PSTATE_ATTACK && state != PSTATE_HURT) {
        state = PSTATE_ATTACK;
        stateTimer = ATTACK_FRAMES;
    }

    if (state == PSTATE_ATTACK) {
        if (stateTimer == 0) {
            state = onGround ? PSTATE_IDLE : PSTATE_FALL;
            stateTimer = 0;
        } else {
            stateTimer--;
        }
    } else if (state == PSTATE_HURT) {
        if (stateTimer >= HURT_FRAMES) {
            state = onGround ? PSTATE_IDLE : PSTATE_FALL;
            stateTimer = 0;
        }
    } else if (!onGround) {
        state = (yvel < 0) ? PSTATE_JUMP : PSTATE_FALL;
    } else if (dx != 0) {
        state = PSTATE_WALK;
    } else {
        state = PSTATE_IDLE;
    }

    if (state == PSTATE_ATTACK) {
        /* countdown timer already advanced above; skip refresh_anim's
           increment so the countdown isn't disturbed */
        if (stateTimer > 12) {
            animFrame = F_ATTACK0;
        } else if (stateTimer > 6) {
            animFrame = F_ATTACK1;
        } else {
            animFrame = F_ATTACK2;
        }
        idleTimer = 0;
    } else {
        refresh_anim();
    }
}

void player_draw(u16 camX) {
    s16 sx = (s16)(px - (s16)camX);
    s16 sy = py;
    u8 flash = (invulnTimer > 0) && (invulnTimer & 2);

    oambuffer[PLAYER_OAM_SLOT].oamattribute =
        OBJ_SIZEL | (facingLeft ? 0x40 : 0x00) | (2 << 4) | (PLAYER_PAL_ROW << 1);
    oambuffer[PLAYER_OAM_SLOT].oamframeid = animFrame;
    oambuffer[PLAYER_OAM_SLOT].oamrefresh = 1;
    if (flash) {
        oambuffer[PLAYER_OAM_SLOT].oamx = 255;
        oambuffer[PLAYER_OAM_SLOT].oamy = 240;
    } else {
        oambuffer[PLAYER_OAM_SLOT].oamx = (u16)sx;
        oambuffer[PLAYER_OAM_SLOT].oamy = (u16)sy;
    }
    oamDynamic32Draw(PLAYER_OAM_SLOT);

    // Sleep Zs are only used by the title/intro scene.
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamx = 255;
    oambuffer[PLAYER_SLEEP_Z_OAM_SLOT].oamy = 240;
    oamDynamic16Draw(PLAYER_SLEEP_Z_OAM_SLOT);

    if (!scriptedPose && state == PSTATE_ATTACK &&
        stateTimer >= ATTACK_ACTIVE_LO && stateTimer <= ATTACK_ACTIVE_HI) {
        s16 ex = facingLeft ? (s16)(sx - 14) : (s16)(sx + 26);
        s16 ey = (s16)(sy + 6);
        oambuffer[PLAYER_SLASH_OAM_SLOT].oamframeid =
            (stateTimer > 5) ? SLASH_F_CRESCENT : SLASH_F_BURST;
        oambuffer[PLAYER_SLASH_OAM_SLOT].oamattribute =
            OBJ_SIZES | (facingLeft ? 0x40 : 0x00) | (2 << 4) | (SLASH_PAL_ROW << 1);
        oambuffer[PLAYER_SLASH_OAM_SLOT].oamrefresh = 1;
        oambuffer[PLAYER_SLASH_OAM_SLOT].oamx = (u16)ex;
        oambuffer[PLAYER_SLASH_OAM_SLOT].oamy = (u16)ey;
    } else {
        oambuffer[PLAYER_SLASH_OAM_SLOT].oamx = 255;
        oambuffer[PLAYER_SLASH_OAM_SLOT].oamy = 240;
    }
    oamDynamic16Draw(PLAYER_SLASH_OAM_SLOT);
}

s16 player_get_x(void) {
    return px;
}

s16 player_get_y(void) {
    return py;
}

void player_get_hurtbox(s16 *x, s16 *y, u8 *w, u8 *h) {
    *x = (s16)(px + 7);
    *y = (s16)(py + 4);
    *w = 18;
    *h = 26;
}

u8 player_is_attack_active(void) {
    return (!scriptedPose && state == PSTATE_ATTACK &&
            stateTimer >= ATTACK_ACTIVE_LO && stateTimer <= ATTACK_ACTIVE_HI);
}

void player_get_attack_box(s16 *x, s16 *y, u8 *w, u8 *h) {
    *x = facingLeft ? (s16)(px - 12) : (s16)(px + PLAYER_SPRITE_W - 4);
    *y = (s16)(py + 6);
    *w = 16;
    *h = 16;
}

void player_take_damage(s8 amount, s16 sourceX) {
    if (invulnTimer > 0 || hearts <= 0) {
        return;
    }
    hearts = (s8)(hearts - amount);
    if (hearts < 0) {
        hearts = 0;
    }
    invulnTimer = INVULN_FRAMES;
    state = PSTATE_HURT;
    stateTimer = 0;
    facingLeft = (sourceX > px) ? 1 : 0;
    yvel = KNOCKBACK_Y;
    {
        u8 hitWall;
        s16 push = facingLeft ? (s16)(-KNOCKBACK_X * 4) : (s16)(KNOCKBACK_X * 4);
        px = collision_resolve_x(px, py, PLAYER_SPRITE_W, PLAYER_SPRITE_H, push, &hitWall);
    }
}

u8 player_get_hearts(void) {
    return (u8)hearts;
}

u8 player_is_dead(void) {
    return hearts <= 0;
}
