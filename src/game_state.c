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

typedef enum { STATE_TITLE, STATE_INTRO, STATE_GAMEPLAY } TopState;

typedef enum {
    INTRO_SLEEP,
    INTRO_QUAKE,
    INTRO_WAKE,
    INTRO_BACK_TO_SLEEP,
    INTRO_SCARAB,
    INTRO_CHASE,
    INTRO_DONE
} IntroBeat;

static TopState topState;
static IntroBeat introBeat;
static u16 beatTimer;
static u8 gameplayWon;

static void show_picture(u8 *til, u16 tilSize, u8 *pal, u16 palSize, u8 *map, u16 mapSize) {
    bgInitTileSet(LEVEL_BG, til, pal, LEVEL_BG_PAL_ROW, tilSize, palSize, BG_16COLORS, LEVEL_BG_TILE_ADDR);
    bgInitMapSet(LEVEL_BG, map, mapSize, SC_32x32, LEVEL_BG_MAP_ADDR);
    bgSetEnable(LEVEL_BG);
    setScreenOn();
}

static void enter_title(void) {
    topState = STATE_TITLE;
    show_picture(
        (u8 *)&title_til, (u16)(&title_tilend - &title_til),
        (u8 *)&title_pal, (u16)(&title_palend - &title_pal),
        (u8 *)&title_map, (u16)(&title_mapend - &title_map)
    );
}

static void enter_intro(void) {
    topState = STATE_INTRO;
    introBeat = INTRO_SLEEP;
    beatTimer = 100;

    show_picture(
        (u8 *)&intro_til, (u16)(&intro_tilend - &intro_til),
        (u8 *)&intro_pal, (u16)(&intro_palend - &intro_pal),
        (u8 *)&intro_map, (u16)(&intro_mapend - &intro_map)
    );

}

static void enter_gameplay(void) {
    topState = STATE_GAMEPLAY;
    gameplayWon = 0;

    bgSetScroll(LEVEL_BG, 0, 0);
    level_load();
    camera_init(LEVEL1_PIXEL_WIDTH);
    player_init(LEVEL1_PLAYER_START_X, LEVEL1_PLAYER_START_Y);
    enemy_init_level1();

}

void game_state_init(void) {
    enter_title();
}

static void update_title(void) {
    u16 pressed = input_pressed();
    if (pressed & (KEY_START | KEY_A)) {
        enter_intro();
    }
}

static void update_intro(void) {
    u16 pressed = input_pressed();

    if (pressed & KEY_START) {
        enter_gameplay();
        return;
    }

    if (introBeat == INTRO_QUAKE) {
        s16 shakeX = (beatTimer & 1) ? 2 : 0;
        s16 shakeY = (beatTimer & 2) ? 1 : 0;
        bgSetScroll(LEVEL_BG, (u16)shakeX, (u16)shakeY);
    }

    if (beatTimer > 0) {
        beatTimer--;
        return;
    }

    switch (introBeat) {
        case INTRO_SLEEP:
            introBeat = INTRO_QUAKE;
            beatTimer = 40;
            break;
        case INTRO_QUAKE:
            bgSetScroll(LEVEL_BG, 0, 0);
            introBeat = INTRO_WAKE;
            beatTimer = 80;
            break;
        case INTRO_WAKE:
            introBeat = INTRO_BACK_TO_SLEEP;
            beatTimer = 60;
            break;
        case INTRO_BACK_TO_SLEEP:
            introBeat = INTRO_SCARAB;
            beatTimer = 60;
            break;
        case INTRO_SCARAB:
            introBeat = INTRO_CHASE;
            beatTimer = 70;
            break;
        case INTRO_CHASE:
            introBeat = INTRO_DONE;
            enter_gameplay();
            break;
        default:
            break;
    }
}

static void update_gameplay(void) {
    if (gameplayWon) {
        return;
    }

    player_update();
    enemy_update_all(player_get_x());
    combat_resolve();
    camera_follow(player_get_x());

    if (player_is_dead()) {
        enter_gameplay();
        return;
    }

    if (enemy_boss_defeated()) {
        gameplayWon = 1;
    }

    player_draw(camera_get_x());
    enemy_draw_all(camera_get_x());
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
    }
}
