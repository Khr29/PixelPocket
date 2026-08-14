#include "game_state.h"
#include "input.h"
#include "text.h"
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

#define INTRO_PLAYER_X 76
#define INTRO_PLAYER_Y 140

static TopState topState;
static IntroBeat introBeat;
static u16 beatTimer;
static u8 gameplayWon;
static u8 lastHudHearts;
static u8 hudBossShown;
static u8 hudWonDrawn;

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
    text_clear();
    text_commit();
}

static void set_intro_text(const char *line1, const char *line2) {
    text_clear();
    if (line1) {
        text_draw(2, 24, line1, TEXT_CREAM);
    }
    if (line2) {
        text_draw(2, 25, line2, TEXT_CREAM);
    }
    text_commit();
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

    player_init(INTRO_PLAYER_X, INTRO_PLAYER_Y);
    player_set_pose(PLAYER_FRAME_SLEEP0, 0);
    set_intro_text(0, 0);
}

static void enter_gameplay(void) {
    topState = STATE_GAMEPLAY;
    gameplayWon = 0;
    lastHudHearts = 0xFF;
    hudBossShown = 0;
    hudWonDrawn = 0;

    bgSetScroll(LEVEL_BG, 0, 0);
    level_load();
    camera_init(LEVEL1_PIXEL_WIDTH);
    player_init(LEVEL1_PLAYER_START_X, LEVEL1_PLAYER_START_Y);
    enemy_init_level1();

    text_clear();
    text_commit();
}

void game_state_init(void) {
    text_init();
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
            set_intro_text("...?", 0);
            break;
        case INTRO_QUAKE:
            bgSetScroll(LEVEL_BG, 0, 0);
            introBeat = INTRO_WAKE;
            beatTimer = 80;
            player_set_pose(PLAYER_FRAME_IDLE0, 0);
            set_intro_text("MRRR...", "FIVE MORE MINUTES.");
            break;
        case INTRO_WAKE:
            introBeat = INTRO_BACK_TO_SLEEP;
            beatTimer = 60;
            player_set_pose(PLAYER_FRAME_SLEEP0, 0);
            set_intro_text(0, 0);
            break;
        case INTRO_BACK_TO_SLEEP:
            introBeat = INTRO_SCARAB;
            beatTimer = 60;
            player_set_pose(PLAYER_FRAME_IDLE0, 0);
            set_intro_text("A SCARAB LANDS ON", "YOUR FACE!");
            break;
        case INTRO_SCARAB:
            introBeat = INTRO_CHASE;
            beatTimer = 70;
            player_set_pose(PLAYER_FRAME_IDLE0, 0);
            set_intro_text("GET BACK HERE!", 0);
            break;
        case INTRO_CHASE:
            introBeat = INTRO_DONE;
            enter_gameplay();
            break;
        default:
            break;
    }
}

static void draw_hud(void) {
    u8 hearts = player_get_hearts();
    u8 bossShown = enemy_is_active(ENEMY_BOSS) && enemy_boss_defeated() == 0 &&
                   (player_get_x() >= LEVEL1_BOSS_MINX - 64);

    if (hearts == lastHudHearts && bossShown == hudBossShown && gameplayWon == hudWonDrawn) {
        return;
    }
    lastHudHearts = hearts;
    hudBossShown = bossShown;
    hudWonDrawn = gameplayWon;

    text_clear();
    text_draw(1, 1, "HP", TEXT_TEAL);
    text_draw_dots(4, 1, PLAYER_MAX_HEARTS, hearts, TEXT_ORANGE);

    if (gameplayWon) {
        text_draw(9, 13, "THE GOLDEN SCORPION", TEXT_GOLD);
        text_draw(12, 14, "FALLS.", TEXT_GOLD);
    }
    text_commit();
}

static void update_gameplay(void) {
    if (gameplayWon) {
        draw_hud();
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
    draw_hud();
}

void game_state_update(void) {
    switch (topState) {
        case STATE_TITLE:
            update_title();
            break;
        case STATE_INTRO:
            update_intro();
            player_draw(0);
            break;
        case STATE_GAMEPLAY:
            update_gameplay();
            break;
    }
}
