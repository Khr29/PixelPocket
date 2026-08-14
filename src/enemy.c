#include "enemy.h"
#include "level.h"
#include "collision.h"

extern char scarab_til, scarab_tilend;
extern char scarab_pal, scarab_palend;
extern char snake_til, snake_tilend;
extern char snake_pal, snake_palend;
extern char mummy_til, mummy_tilend;
extern char mummy_pal, mummy_palend;
extern char rat_til, rat_tilend;
extern char rat_pal, rat_palend;
extern char bossscorpion_til, bossscorpion_tilend;
extern char bossscorpion_pal, bossscorpion_palend;

#define SCARAB_OAM_SLOT 2
#define SNAKE_OAM_SLOT 3
#define MUMMY_OAM_SLOT 4
#define RAT_OAM_SLOT 5
#define BOSS_OAM_TL 6
#define BOSS_OAM_TR 7
#define BOSS_OAM_BL 8
#define BOSS_OAM_BR 9

#define SCARAB_PAL_ROW 2
#define SNAKE_PAL_ROW 3
#define MUMMY_PAL_ROW 4
#define RAT_PAL_ROW 5
#define BOSS_PAL_ROW 6

#define SPRITE_W 32
#define SPRITE_H 32
#define HURT_W 22
#define HURT_H 22

#define BOSS_W 64
#define BOSS_H 64
#define BOSS_HURT_W 44
#define BOSS_HURT_H 40

#define BOSS_MAX_HP 5
#define BOSS_ENGAGE_MARGIN 48
#define BOSS_WALK_FRAMES 100
#define BOSS_TELEGRAPH_FRAMES 30
#define BOSS_ATTACK_FRAMES 22
#define BOSS_VULNERABLE_FRAMES 60
#define BOSS_HURT_FLASH_FRAMES 16

typedef enum {
    BOSS_ASLEEP,
    BOSS_WALK,
    BOSS_TELEGRAPH,
    BOSS_ATTACK,
    BOSS_VULNERABLE,
    BOSS_DEFEATED
} BossState;

typedef enum { AI_PATROL, AI_CHASE } AiKind;

static s16 ex[ENEMY_COUNT], ey[ENEMY_COUNT];
static s16 eminx[ENEMY_COUNT], emaxx[ENEMY_COUNT];
static s8 ehp[ENEMY_COUNT];
static u8 eactive[ENEMY_COUNT];
static u8 ehurtTimer[ENEMY_COUNT];
static u8 eanimTimer[ENEMY_COUNT];
static u8 efacingLeft[ENEMY_COUNT];
static u8 espeedDiv[ENEMY_COUNT];
static AiKind eai[ENEMY_COUNT];

static u8 bossState;
static u16 bossTimer;

static void enemy_setup_common(EnemyId id, u8 *til, u8 *pal, u8 palRow, u8 oamSlot,
                                s16 x, s16 y, s16 minx, s16 maxx, u8 speedDiv, AiKind ai) {
    ex[id] = x;
    ey[id] = y;
    eminx[id] = minx;
    emaxx[id] = maxx;
    ehp[id] = 1;
    eactive[id] = 1;
    ehurtTimer[id] = 0;
    eanimTimer[id] = 0;
    efacingLeft[id] = 0;
    espeedDiv[id] = speedDiv;
    eai[id] = ai;

    setPalette(pal, 128 + palRow * 16, 16 * 2);
    oambuffer[oamSlot].oamgraphics = til;
    oambuffer[oamSlot].oamattribute = OBJ_SIZEL | (2 << 4) | (palRow << 1);
    oambuffer[oamSlot].oamframeid = 0;
    oambuffer[oamSlot].oamrefresh = 1;
}

void enemy_init_level1(void) {
    enemy_setup_common(ENEMY_SCARAB, (u8 *)&scarab_til, (u8 *)&scarab_pal, SCARAB_PAL_ROW,
                        SCARAB_OAM_SLOT, LEVEL1_SCARAB_X, LEVEL1_FLOOR_TOP_ROW * 8 - SPRITE_H,
                        LEVEL1_SCARAB_MINX, LEVEL1_SCARAB_MAXX, 3, AI_PATROL);
    enemy_setup_common(ENEMY_SNAKE, (u8 *)&snake_til, (u8 *)&snake_pal, SNAKE_PAL_ROW,
                        SNAKE_OAM_SLOT, LEVEL1_SNAKE_X, LEVEL1_FLOOR_TOP_ROW * 8 - SPRITE_H,
                        LEVEL1_SNAKE_MINX, LEVEL1_SNAKE_MAXX, 3, AI_PATROL);
    enemy_setup_common(ENEMY_MUMMY, (u8 *)&mummy_til, (u8 *)&mummy_pal, MUMMY_PAL_ROW,
                        MUMMY_OAM_SLOT, LEVEL1_MUMMY_X, LEVEL1_FLOOR_TOP_ROW * 8 - SPRITE_H,
                        LEVEL1_MUMMY_MINX, LEVEL1_MUMMY_MAXX, 6, AI_CHASE);
    enemy_setup_common(ENEMY_RAT, (u8 *)&rat_til, (u8 *)&rat_pal, RAT_PAL_ROW,
                        RAT_OAM_SLOT, LEVEL1_RAT_X, LEVEL1_FLOOR_TOP_ROW * 8 - SPRITE_H,
                        LEVEL1_RAT_MINX, LEVEL1_RAT_MAXX, 2, AI_PATROL);

    ex[ENEMY_BOSS] = LEVEL1_BOSS_X;
    ey[ENEMY_BOSS] = LEVEL1_BOSS_Y;
    eminx[ENEMY_BOSS] = LEVEL1_BOSS_MINX;
    emaxx[ENEMY_BOSS] = LEVEL1_BOSS_MAXX;
    ehp[ENEMY_BOSS] = BOSS_MAX_HP;
    eactive[ENEMY_BOSS] = 1;
    ehurtTimer[ENEMY_BOSS] = 0;
    eanimTimer[ENEMY_BOSS] = 0;
    efacingLeft[ENEMY_BOSS] = 1;
    bossState = BOSS_ASLEEP;
    bossTimer = 0;

    setPalette((u8 *)&bossscorpion_pal, 128 + BOSS_PAL_ROW * 16, 16 * 2);
    {
        u8 slot;
        for (slot = BOSS_OAM_TL; slot <= BOSS_OAM_BR; slot++) {
            oambuffer[slot].oamgraphics = (u8 *)&bossscorpion_til;
            oambuffer[slot].oamattribute = OBJ_SIZEL | (2 << 4) | (BOSS_PAL_ROW << 1);
            oambuffer[slot].oamframeid = 0;
            oambuffer[slot].oamrefresh = 1;
        }
    }
}

static void patrol_update(EnemyId id) {
    if (!eactive[id] || ehurtTimer[id] > 0) {
        return;
    }
    eanimTimer[id]++;
    if ((eanimTimer[id] % espeedDiv[id]) == 0) {
        if (efacingLeft[id]) {
            if (ex[id] <= eminx[id]) {
                efacingLeft[id] = 0;
            } else {
                ex[id]--;
            }
        } else {
            if (ex[id] >= emaxx[id]) {
                efacingLeft[id] = 1;
            } else {
                ex[id]++;
            }
        }
    }
}

static void chase_update(EnemyId id, s16 playerX) {
    if (!eactive[id] || ehurtTimer[id] > 0) {
        return;
    }
    eanimTimer[id]++;
    if ((eanimTimer[id] % espeedDiv[id]) != 0) {
        return;
    }
    if (playerX < ex[id]) {
        efacingLeft[id] = 1;
        if (ex[id] > eminx[id]) {
            ex[id]--;
        }
    } else if (playerX > ex[id]) {
        efacingLeft[id] = 0;
        if (ex[id] < emaxx[id]) {
            ex[id]++;
        }
    }
}

static void boss_update(s16 playerX) {
    if (bossState == BOSS_DEFEATED) {
        return;
    }
    if (ehurtTimer[ENEMY_BOSS] > 0) {
        ehurtTimer[ENEMY_BOSS]--;
    }

    if (bossState == BOSS_ASLEEP) {
        if (playerX >= eminx[ENEMY_BOSS] - BOSS_ENGAGE_MARGIN) {
            bossState = BOSS_WALK;
            bossTimer = BOSS_WALK_FRAMES;
        }
        return;
    }

    efacingLeft[ENEMY_BOSS] = (playerX < ex[ENEMY_BOSS]) ? 1 : 0;

    switch (bossState) {
        case BOSS_WALK:
            eanimTimer[ENEMY_BOSS]++;
            if ((eanimTimer[ENEMY_BOSS] & 3) == 0) {
                if (efacingLeft[ENEMY_BOSS] && ex[ENEMY_BOSS] > eminx[ENEMY_BOSS]) {
                    ex[ENEMY_BOSS]--;
                } else if (!efacingLeft[ENEMY_BOSS] && ex[ENEMY_BOSS] < emaxx[ENEMY_BOSS]) {
                    ex[ENEMY_BOSS]++;
                }
            }
            if (bossTimer > 0) {
                bossTimer--;
            } else {
                bossState = BOSS_TELEGRAPH;
                bossTimer = BOSS_TELEGRAPH_FRAMES;
            }
            break;
        case BOSS_TELEGRAPH:
            if (bossTimer > 0) {
                bossTimer--;
            } else {
                bossState = BOSS_ATTACK;
                bossTimer = BOSS_ATTACK_FRAMES;
            }
            break;
        case BOSS_ATTACK:
            if (bossTimer > 0) {
                bossTimer--;
            } else {
                bossState = BOSS_VULNERABLE;
                bossTimer = BOSS_VULNERABLE_FRAMES;
            }
            break;
        case BOSS_VULNERABLE:
            if (bossTimer > 0) {
                bossTimer--;
            } else {
                bossState = BOSS_WALK;
                bossTimer = BOSS_WALK_FRAMES;
            }
            break;
        default:
            break;
    }
}

void enemy_update_all(s16 playerX) {
    u8 i;
    for (i = 0; i < ENEMY_COUNT - 1; i++) {
        if (eai[i] == AI_PATROL) {
            patrol_update((EnemyId)i);
        } else {
            chase_update((EnemyId)i, playerX);
        }
        if (eactive[i] && ehurtTimer[i] > 0) {
            ehurtTimer[i]--;
        }
    }
    boss_update(playerX);
}

static void draw_small_enemy(EnemyId id, u8 oamSlot, u8 palRow, u16 camX) {
    if (!eactive[id]) {
        oambuffer[oamSlot].oamx = 255;
        oambuffer[oamSlot].oamy = 240;
        oamDynamic32Draw(oamSlot);
        return;
    }
    {
        u8 flash = (ehurtTimer[id] > 0) && (ehurtTimer[id] & 2);
        u8 frame = (u8)((eanimTimer[id] / 10) & 1);
        s16 sx = (s16)(ex[id] - (s16)camX);
        s16 sy = ey[id];

        oambuffer[oamSlot].oamattribute =
            OBJ_SIZEL | (efacingLeft[id] ? 0x40 : 0x00) | (2 << 4) | (palRow << 1);
        oambuffer[oamSlot].oamframeid = frame;
        oambuffer[oamSlot].oamrefresh = 1;
        if (flash) {
            oambuffer[oamSlot].oamx = 255;
            oambuffer[oamSlot].oamy = 240;
        } else {
            oambuffer[oamSlot].oamx = (u16)sx;
            oambuffer[oamSlot].oamy = (u16)sy;
        }
        oamDynamic32Draw(oamSlot);
    }
}

// Boss quadrant frame ids within boss_scorpion.png's 4x4 grid of 32x32
// cells: each state occupies a 2x2 block; TL,TR,BL,BR listed in that order.
static void boss_quadrant_frames(u8 *tl, u8 *tr, u8 *bl, u8 *br) {
    u8 base;
    switch (bossState) {
        case BOSS_TELEGRAPH:
        case BOSS_ATTACK:
            base = 8;
            break;
        case BOSS_VULNERABLE:
            base = 10;
            break;
        case BOSS_WALK:
        case BOSS_ASLEEP:
        default:
            base = (ehurtTimer[ENEMY_BOSS] > 0) ? 10 : ((eanimTimer[ENEMY_BOSS] & 16) ? 2 : 0);
            break;
    }
    *tl = base;
    *tr = (u8)(base + 1);
    *bl = (u8)(base + 4);
    *br = (u8)(base + 5);
}

static void draw_boss(u16 camX) {
    if (bossState == BOSS_DEFEATED) {
        u8 slot;
        for (slot = BOSS_OAM_TL; slot <= BOSS_OAM_BR; slot++) {
            oambuffer[slot].oamx = 255;
            oambuffer[slot].oamy = 240;
            oamDynamic32Draw(slot);
        }
        return;
    }

    {
        u8 tl, tr, bl, br;
        u8 flash = (ehurtTimer[ENEMY_BOSS] > 0) && (ehurtTimer[ENEMY_BOSS] & 2);
        s16 sx = (s16)(ex[ENEMY_BOSS] - (s16)camX);
        s16 sy = ey[ENEMY_BOSS];
        u8 attr = OBJ_SIZEL | (2 << 4) | (BOSS_PAL_ROW << 1);

        boss_quadrant_frames(&tl, &tr, &bl, &br);

        if (flash) {
            u8 slot;
            for (slot = BOSS_OAM_TL; slot <= BOSS_OAM_BR; slot++) {
                oambuffer[slot].oamx = 255;
                oambuffer[slot].oamy = 240;
                oamDynamic32Draw(slot);
            }
            return;
        }

        if (!efacingLeft[ENEMY_BOSS]) {
            /* art faces left by default; mirror the 2x2 block for right-facing */
            u8 t = tl; tl = tr; tr = t;
            t = bl; bl = br; br = t;
            attr |= 0x40;
        }

        oambuffer[BOSS_OAM_TL].oamframeid = tl;
        oambuffer[BOSS_OAM_TR].oamframeid = tr;
        oambuffer[BOSS_OAM_BL].oamframeid = bl;
        oambuffer[BOSS_OAM_BR].oamframeid = br;

        oambuffer[BOSS_OAM_TL].oamattribute = attr;
        oambuffer[BOSS_OAM_TR].oamattribute = attr;
        oambuffer[BOSS_OAM_BL].oamattribute = attr;
        oambuffer[BOSS_OAM_BR].oamattribute = attr;

        oambuffer[BOSS_OAM_TL].oamx = (u16)sx;
        oambuffer[BOSS_OAM_TL].oamy = (u16)sy;
        oambuffer[BOSS_OAM_TR].oamx = (u16)(sx + 32);
        oambuffer[BOSS_OAM_TR].oamy = (u16)sy;
        oambuffer[BOSS_OAM_BL].oamx = (u16)sx;
        oambuffer[BOSS_OAM_BL].oamy = (u16)(sy + 32);
        oambuffer[BOSS_OAM_BR].oamx = (u16)(sx + 32);
        oambuffer[BOSS_OAM_BR].oamy = (u16)(sy + 32);

        oambuffer[BOSS_OAM_TL].oamrefresh = 1;
        oambuffer[BOSS_OAM_TR].oamrefresh = 1;
        oambuffer[BOSS_OAM_BL].oamrefresh = 1;
        oambuffer[BOSS_OAM_BR].oamrefresh = 1;

        oamDynamic32Draw(BOSS_OAM_TL);
        oamDynamic32Draw(BOSS_OAM_TR);
        oamDynamic32Draw(BOSS_OAM_BL);
        oamDynamic32Draw(BOSS_OAM_BR);
    }
}

void enemy_draw_all(u16 camX) {
    draw_small_enemy(ENEMY_SCARAB, SCARAB_OAM_SLOT, SCARAB_PAL_ROW, camX);
    draw_small_enemy(ENEMY_SNAKE, SNAKE_OAM_SLOT, SNAKE_PAL_ROW, camX);
    draw_small_enemy(ENEMY_MUMMY, MUMMY_OAM_SLOT, MUMMY_PAL_ROW, camX);
    draw_small_enemy(ENEMY_RAT, RAT_OAM_SLOT, RAT_PAL_ROW, camX);
    draw_boss(camX);
}

void enemy_hide_all(void) {
    u8 slot;

    /* oamDynamic32Draw() uploads whatever oambuffer[slot].oamgraphics points
       to -- these slots only get a valid pointer via enemy_setup_common()
       (called from enemy_init_level1()), so that has to run first or the
       draw call below would DMA garbage into the shared sprite VRAM pool. */
    enemy_init_level1();

    for (slot = SCARAB_OAM_SLOT; slot <= BOSS_OAM_BR; slot++) {
        oambuffer[slot].oamx = 255;
        oambuffer[slot].oamy = 240;
        oamDynamic32Draw(slot);
    }
}

u8 enemy_is_active(EnemyId id) {
    if (id == ENEMY_BOSS) {
        return bossState != BOSS_DEFEATED;
    }
    return eactive[id];
}

void enemy_get_hurtbox(EnemyId id, s16 *x, s16 *y, u8 *w, u8 *h) {
    if (id == ENEMY_BOSS) {
        *x = (s16)(ex[id] + 10);
        *y = (s16)(ey[id] + 12);
        *w = BOSS_HURT_W;
        *h = BOSS_HURT_H;
        return;
    }
    *x = (s16)(ex[id] + 5);
    *y = (s16)(ey[id] + 8);
    *w = HURT_W;
    *h = HURT_H;
}

u8 enemy_is_dangerous(EnemyId id) {
    if (!enemy_is_active(id)) {
        return 0;
    }
    if (ehurtTimer[id] > 0) {
        return 0;
    }
    if (id == ENEMY_BOSS) {
        return bossState == BOSS_ATTACK || bossState == BOSS_WALK;
    }
    return 1;
}

void enemy_hurt(EnemyId id, s8 amount) {
    if (!enemy_is_active(id) || ehurtTimer[id] > 0) {
        return;
    }
    if (id == ENEMY_BOSS && bossState != BOSS_VULNERABLE) {
        return;
    }

    ehp[id] = (s8)(ehp[id] - amount);
    ehurtTimer[id] = (id == ENEMY_BOSS) ? BOSS_HURT_FLASH_FRAMES : 20;

    if (ehp[id] <= 0) {
        if (id == ENEMY_BOSS) {
            bossState = BOSS_DEFEATED;
        } else {
            eactive[id] = 0;
        }
    }
}

u8 enemy_boss_defeated(void) {
    return bossState == BOSS_DEFEATED;
}
