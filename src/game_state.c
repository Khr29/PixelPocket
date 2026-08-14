#include "game_state.h"
#include "input.h"
#include "level.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "combat.h"

extern char title_til, title_tilend;
extern char title_pal, title_palend;
extern char title_map, title_mapend;
extern char intro_til, intro_tilend;
extern char intro_pal, intro_palend;
extern char intro_map, intro_mapend;

/*
 * Top-level flow:
 *   TITLE -> INTRO -> GAMEPLAY -> PAUSED
 *
 * The artwork is intentionally kept as the approved reference-derived
 * backgrounds. Character motion is done with real OAM sprites over them.
 */
typedef enum { STATE_TITLE, STATE_INTRO, STATE_GAMEPLAY, STATE_PAUSED } TopState;

typedef enum {
    INTRO_SLEEP,
    INTRO_WAKE,
    INTRO_WALK,
    INTRO_DONE
} IntroBeat;

static TopState topState;
static IntroBeat introBeat;
static u16 stateTimer;
static u8 titlePhase;
static s16 introX;
static u8 gameplayRevealTimer;

static void show_picture(u8 *til, u16 tilSize, u8 *pal, u16 palSize, u8 *map, u16 mapSize) {
    bgInitTileSet(LEVEL_BG, til, pal, LEVEL_BG_PAL_ROW, tilSize, palSize, BG_16COLORS, LEVEL_BG_TILE_ADDR);
    bgInitMapSet(LEVEL_BG, map, mapSize, SC_32x32, LEVEL_BG_MAP_ADDR);
    bgSetScroll(LEVEL_BG, 0, 0);
    bgSetEnable(LEVEL_BG);
    setScreenOn();
}

static void enter_title(void) {
    topState = STATE_TITLE;
    stateTimer = 0;
    titlePhase = 0;

    show_picture(
        (u8 *)&title_til, (u16)(&title_tilend - &title_til),
        (u8 *)&title_pal, (u16)(&title_palend - &title_pal),
        (u8 *)&title_map, (u16)(&title_mapend - &title_map)
    );

    /* Sleeping Yaqub is a real sprite, not baked into the title artwork. */
    player_init(112, 144);
    player_set_pose(PLAYER_FRAME_SLEEP0, 0);
}

static void enter_intro(void) {
    topState = STATE_INTRO;
    introBeat = INTRO_SLEEP;
    stateTimer = 0;
    introX = 88;

    show_picture(
        (u8 *)&intro_til, (u16)(&intro_tilend - &intro_til),
        (u8 *)&intro_pal, (u16)(&intro_palend - &intro_pal),
        (u8 *)&intro_map, (u16)(&intro_mapend - &intro_map)
    );

    player_init(introX, LEVEL1_PLAYER_START_Y);
    player_set_pose(PLAYER_FRAME_SLEEP0, 0);
}

static void enter_gameplay(s16 spawnX, s16 spawnY) {
    topState = STATE_GAMEPLAY;
    stateTimer = 0;
    gameplayRevealTimer = 50;

    level_load();
    camera_init(LEVEL1_PIXEL_WIDTH);
    player_init(spawnX, spawnY);
    enemy_init_level1();

    /* Begin with the player slightly left of centre; the camera reveal then
       eases to its normal follow position without scaling the screen. */
    bgSetScroll(LEVEL_BG, 32, 0);
}

void game_state_init(void) {
    enter_title();
}

static void draw_sleeping_scene(u8 phase) {
    /* Small 1-pixel bob: the cat feels alive without a distracting blink. */
    s16 bob = ((phase >> 5) & 1) ? 1 : 0;
    player_set_position(112, (s16)(144 + bob));
    player_set_pose(PLAYER_FRAME_SLEEP0, 0);
    player_draw(0);
    player_draw_sleep_z(112, (s16)(144 + bob), phase);
}

static void update_title(void) {
    u16 pressed = input_pressed();

    titlePhase++;
    draw_sleeping_scene(titlePhase);

    if (pressed & (KEY_START | KEY_A)) {
        enter_intro();
    }
}

static void update_intro(void) {
    u16 pressed = input_pressed();

    if (pressed & KEY_START) {
        enter_gameplay(96, LEVEL1_PLAYER_START_Y);
        return;
    }

    switch (introBeat) {
        case INTRO_SLEEP:
            player_set_position(introX, LEVEL1_PLAYER_START_Y);
            player_set_pose(PLAYER_FRAME_SLEEP0, 0);
            player_draw(0);
            player_draw_sleep_z(introX, LEVEL1_PLAYER_START_Y, (u8)stateTimer);

            if (stateTimer >= 150) {
                introBeat = INTRO_WAKE;
                stateTimer = 0;
            }
            break;

        case INTRO_WAKE:
        {
            u8 frame;
            /* Idle frames sell the wake-up better than an abrupt teleport. */
            if (stateTimer < 15) frame = PLAYER_FRAME_IDLE0;
            else if (stateTimer < 30) frame = PLAYER_FRAME_IDLE0 + 1;
            else if (stateTimer < 45) frame = PLAYER_FRAME_IDLE0 + 2;
            else frame = PLAYER_FRAME_IDLE0;

            player_set_position(introX, LEVEL1_PLAYER_START_Y);
            player_set_pose(frame, 0);
            player_draw(0);

            if (stateTimer >= 55) {
                introBeat = INTRO_WALK;
                stateTimer = 0;
            }
            break;
        }

        case INTRO_WALK:
        {
            u8 frame = (u8)(PLAYER_FRAME_WALK0 + ((stateTimer >> 2) & 3));
            introX = (s16)(88 + (stateTimer >> 1));
            player_set_position(introX, LEVEL1_PLAYER_START_Y);
            player_set_pose(frame, 0);
            player_draw(0);

            /* Let the player take control after a short cinematic walk. */
            if (stateTimer >= 80) {
                enter_gameplay(128, LEVEL1_PLAYER_START_Y);
            }
            break;
        }

        default:
            enter_gameplay(128, LEVEL1_PLAYER_START_Y);
            break;
    }

    stateTimer++;
}

static void update_gameplay(void) {
    u16 pressed = input_pressed();

    if (pressed & KEY_START) {
        topState = STATE_PAUSED;
        return;
    }

    player_update();
    enemy_update_all(player_get_x());
    combat_resolve();

    if (gameplayRevealTimer > 0) {
        /* Smooth camera reveal: start 32px to the right and ease back to
           the normal camera origin. This creates a small cinematic reveal
           without scaling the 256x224 image. */
        u16 scrollX = (u16)(((u16)gameplayRevealTimer * 32) / 50);
        bgSetScroll(LEVEL_BG, scrollX, 0);
        gameplayRevealTimer--;
    } else {
        camera_follow(player_get_x());
    }

    if (player_is_dead()) {
        enter_gameplay(128, LEVEL1_PLAYER_START_Y);
        return;
    }

    player_draw(camera_get_x());
    enemy_draw_all(camera_get_x());
}

static void update_paused(void) {
    /* Keep the game frozen and return to gameplay with START. */
    if (input_pressed() & KEY_START) {
        topState = STATE_GAMEPLAY;
    }
}

void game_state_update(void) {
    switch (topState) {
        case STATE_TITLE:
            update_title();
            break;
        case STATE_INTRO:
            update_intro();
            break;
        case STATE_GAMEPLAY:
            update_gameplay();
            break;
        case STATE_PAUSED:
            update_paused();
            break;
    }
}
