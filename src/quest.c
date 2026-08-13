#include <snes.h>
#include "quest.h"
#include "quest_data.h"
#include "text.h"

// ---------------------------------------------------------------------------
// Chrome (BG0): one fully pre-rendered picture per screen. Swapping screens
// re-uploads tiles+map to the same fixed VRAM address (only one screen is
// ever shown at a time) -- see [[pvsneslib_bginit_blanks_screen]], every
// chrome_show() ends with setScreenOn() to undo the forced blank.
// ---------------------------------------------------------------------------

extern char dashboard_til, dashboard_tilend, dashboard_map, dashboard_mapend, dashboard_pal, dashboard_palend;
extern char questdetails_til, questdetails_tilend, questdetails_map, questdetails_mapend, questdetails_pal, questdetails_palend;
extern char questcomplete_til, questcomplete_tilend, questcomplete_map, questcomplete_mapend, questcomplete_pal, questcomplete_palend;
extern char stats_til, stats_tilend, stats_map, stats_mapend, stats_pal, stats_palend;

#define CHROME_BG 0
#define CHROME_TILE_ADDR 0x1000
#define CHROME_MAP_ADDR 0x4000
#define CHROME_PALETTE_ENTRY 0

static void chrome_show(char *til, char *tilend, char *pal, char *palend, char *map, char *mapend) {
    bgInitTileSet(
        CHROME_BG, (u8 *)til, (u8 *)pal, CHROME_PALETTE_ENTRY,
        (u16)(tilend - til), (u16)(palend - pal), BG_16COLORS, CHROME_TILE_ADDR
    );
    bgInitMapSet(CHROME_BG, (u8 *)map, (u16)(mapend - map), SC_32x32, CHROME_MAP_ADDR);
    setScreenOn();
}

#define SHOW_DASHBOARD() chrome_show(&dashboard_til, &dashboard_tilend, &dashboard_pal, &dashboard_palend, &dashboard_map, &dashboard_mapend)
#define SHOW_QUEST_DETAILS() chrome_show(&questdetails_til, &questdetails_tilend, &questdetails_pal, &questdetails_palend, &questdetails_map, &questdetails_mapend)
#define SHOW_QUEST_COMPLETE() chrome_show(&questcomplete_til, &questcomplete_tilend, &questcomplete_pal, &questcomplete_palend, &questcomplete_map, &questcomplete_mapend)
#define SHOW_STATS() chrome_show(&stats_til, &stats_tilend, &stats_pal, &stats_palend, &stats_map, &stats_mapend)

// ---------------------------------------------------------------------------
// Player progress. Kept simple on purpose -- see CLAUDE.md.
// ---------------------------------------------------------------------------

static u8 g_level = 12;
static u16 g_totalXP = 12480;
static u16 g_xpIntoLevel = 1240;
static u16 g_xpToNextLevel = 1500;
static u8 g_streak = 7;

static u16 g_statKnowledge = 24;
static u16 g_statCoding = 27;
static u16 g_statCreativity = 12;
static u16 g_statDiscipline = 18;
static u16 g_statExploration = 9;

static const u8 DAY_NUMBER = 7;

static u16 xp_needed_for_level(u8 level) {
    return (u16)(1200 + (u16)level * 25);
}

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

static GameState g_state = STATE_DASHBOARD;
static u8 g_cursor = 0;         // index into QUEST_TODAY
static u8 g_completingIdx = 0;  // which QUEST_TODAY entry is open in details/complete
static u16 g_lastRewardXP = 0;
static bool g_leveledUp = false;

static void bump_stat(QuestCategory cat) {
    switch (cat) {
        case QUEST_CAT_READING:
        case QUEST_CAT_LEARNING:
            g_statKnowledge++;
            break;
        case QUEST_CAT_CODING:
            g_statCoding++;
            break;
        case QUEST_CAT_CREATIVE:
            g_statCreativity++;
            break;
        case QUEST_CAT_LIFE:
            g_statDiscipline++;
            break;
        case QUEST_CAT_EXPLORATION:
            g_statExploration++;
            break;
    }
}

static void complete_quest(Quest *q) {
    if (q->completed) return;
    q->completed = true;

    g_lastRewardXP = q->xp;
    g_totalXP = (u16)(g_totalXP + q->xp);
    g_xpIntoLevel = (u16)(g_xpIntoLevel + q->xp);
    bump_stat(q->category);

    g_leveledUp = false;
    while (g_xpIntoLevel >= g_xpToNextLevel) {
        g_xpIntoLevel = (u16)(g_xpIntoLevel - g_xpToNextLevel);
        g_level++;
        g_xpToNextLevel = xp_needed_for_level(g_level);
        g_leveledUp = true;
    }
}

// ---------------------------------------------------------------------------
// Small render helpers
// ---------------------------------------------------------------------------

static void draw_digits2(u8 col, u8 row, u8 value, u8 color) {
    text_put(col, row, (char)('0' + (value / 10) % 10), color);
    text_put((u8)(col + 1), row, (char)('0' + value % 10), color);
}

static void draw_stars(u8 col, u8 row, u8 difficulty) {
    u8 i;
    for (i = 0; i < 5; i++) {
        text_put((u8)(col + i), row, (char)(i < difficulty ? '#' : '-'), TEXT_GOLD);
    }
}

// ---------------------------------------------------------------------------
// DASHBOARD
// ---------------------------------------------------------------------------

static void render_dashboard(void) {
    u8 i;
    u8 doneCount = 0;

    SHOW_DASHBOARD();
    text_clear();

    text_draw(25, 1, "DAY ", TEXT_CREAM);
    draw_digits2(29, 1, DAY_NUMBER, TEXT_CREAM);

    text_draw_u16(1, 6, g_level, TEXT_GOLD);
    text_draw_u16_comma(25, 6, g_totalXP, TEXT_GOLD);

    text_draw_bar(2, 10, 28, g_xpIntoLevel, g_xpToNextLevel, TEXT_GOLD);
    {
        u8 c = text_draw_u16_comma(13, 12, g_xpIntoLevel, TEXT_CREAM);
        c = text_draw(c, 12, " / ", TEXT_CREAM);
        c = text_draw_u16_comma(c, 12, g_xpToNextLevel, TEXT_CREAM);
        text_draw(c, 12, " XP", TEXT_CREAM);
    }

    draw_digits2(12, 14, g_streak, TEXT_ORANGE);

    for (i = 0; i < QUEST_TODAY_COUNT; i++) {
        Quest *q = &QUEST_TODAY[i];
        u8 row = (u8)(17 + i);
        u8 nameColor = q->completed ? TEXT_MUTED : TEXT_CREAM;
        u8 c;

        text_put(1, row, i == g_cursor ? '>' : ' ', TEXT_GOLD);
        text_put(3, row, q->completed ? '^' : '-', q->completed ? TEXT_GREEN : TEXT_MUTED);
        text_draw(5, row, q->name, nameColor);

        c = (u8)(30 - (1 + text_u16_width(q->xp) + 3)); // "+" + digits + " XP"
        c = text_draw(c, row, "+", TEXT_GOLD);
        c = text_draw_u16(c, row, q->xp, TEXT_GOLD);
        text_draw(c, row, " XP", TEXT_GOLD);

        if (q->completed) doneCount++;
    }

    {
        u8 c = text_draw(1, 21, "TODAY: ", TEXT_TEAL);
        c = text_draw_u16(c, 21, doneCount, TEXT_TEAL);
        c = text_draw(c, 21, " / ", TEXT_TEAL);
        c = text_draw_u16(c, 21, QUEST_TODAY_COUNT, TEXT_TEAL);
        text_draw(c, 21, " COMPLETE", TEXT_TEAL);
    }
    text_draw_dots(26, 21, QUEST_TODAY_COUNT, doneCount, TEXT_GREEN);

    for (i = 0; i < QUEST_SIDE_COUNT; i++) {
        Quest *q = &QUEST_SIDE[i];
        u8 row = (u8)(24 + i);
        u8 c;
        text_draw(1, row, q->name, TEXT_CREAM);
        c = (u8)(30 - (1 + text_u16_width(q->xp) + 3));
        c = text_draw(c, row, "+", TEXT_GOLD);
        c = text_draw_u16(c, row, q->xp, TEXT_GOLD);
        text_draw(c, row, " XP", TEXT_GOLD);
    }

    text_commit();
}

// ---------------------------------------------------------------------------
// QUEST DETAILS
// ---------------------------------------------------------------------------

static void render_quest_details(void) {
    Quest *q = &QUEST_TODAY[g_completingIdx];
    const char *kind = "TODAY'S QUEST";
    u8 c;

    SHOW_QUEST_DETAILS();
    text_clear();

    text_draw((u8)(16 - 6), 1, kind, TEXT_GOLD);
    text_draw(2, 3, q->name, TEXT_CREAM);
    text_draw(2, 5, quest_category_name(q->category), TEXT_TEAL);
    draw_stars(2, 6, q->difficulty);
    text_draw(2, 8, q->description, TEXT_MUTED);

    c = text_draw(9, 13, "+", TEXT_GOLD);
    c = text_draw_u16(c, 13, q->xp, TEXT_GOLD);
    text_draw(c, 13, " XP", TEXT_GOLD);

    text_commit();
}

// ---------------------------------------------------------------------------
// QUEST COMPLETE
// ---------------------------------------------------------------------------

static void render_quest_complete(void) {
    Quest *q = &QUEST_TODAY[g_completingIdx];
    u8 c;

    SHOW_QUEST_COMPLETE();
    text_clear();

    if (g_leveledUp) {
        text_draw(9, 10, "LEVEL UP!", TEXT_GOLD);
        c = text_draw(9, 13, "LEVEL ", TEXT_CREAM);
        text_draw_u16(c, 13, g_level, TEXT_GOLD);
    } else {
        text_draw(8, 10, "QUEST COMPLETE!", TEXT_GOLD);
        c = text_draw(12, 13, "+", TEXT_GOLD);
        c = text_draw_u16(c, 13, g_lastRewardXP, TEXT_GOLD);
        text_draw(c, 13, " XP", TEXT_GOLD);
        c = text_draw(9, 16, quest_category_name(q->category), TEXT_TEAL);
        text_draw(c, 16, " +1", TEXT_TEAL);
    }

    text_commit();
}

// ---------------------------------------------------------------------------
// STATS
// ---------------------------------------------------------------------------

static void render_stats(void) {
    SHOW_STATS();
    text_clear();

    text_draw_u16(24, 4, g_level, TEXT_GOLD);
    text_draw_u16_comma(24, 6, g_totalXP, TEXT_GOLD);
    {
        u8 c = text_draw_u16(24, 7, g_streak, TEXT_ORANGE);
        text_draw(c, 7, " DAYS", TEXT_ORANGE);
    }

    text_draw_u16(24, 10, g_statKnowledge, TEXT_CREAM);
    text_draw_u16(24, 12, g_statCoding, TEXT_CREAM);
    text_draw_u16(24, 14, g_statCreativity, TEXT_CREAM);
    text_draw_u16(24, 15, g_statDiscipline, TEXT_CREAM);
    text_draw_u16(24, 17, g_statExploration, TEXT_CREAM);

    text_commit();
}

// ---------------------------------------------------------------------------
// Input / state machine
// ---------------------------------------------------------------------------

void quest_init(void) {
    setMode(BG_MODE1, 0);
    bgSetDisable(2);
    text_init();

    g_state = STATE_DASHBOARD;
    render_dashboard();
}

void quest_update(u16 padPressed) {
    switch (g_state) {
        case STATE_DASHBOARD:
            if (padPressed & KEY_DOWN) {
                g_cursor = (u8)((g_cursor + 1) % QUEST_TODAY_COUNT);
                render_dashboard();
            } else if (padPressed & KEY_UP) {
                g_cursor = (u8)((g_cursor + QUEST_TODAY_COUNT - 1) % QUEST_TODAY_COUNT);
                render_dashboard();
            } else if (padPressed & KEY_A) {
                g_completingIdx = g_cursor;
                g_state = STATE_QUEST_DETAILS;
                render_quest_details();
            } else if (padPressed & KEY_START) {
                g_state = STATE_STATS;
                render_stats();
            }
            break;

        case STATE_QUEST_DETAILS:
            if (padPressed & KEY_B) {
                g_state = STATE_DASHBOARD;
                render_dashboard();
            } else if (padPressed & KEY_A) {
                complete_quest(&QUEST_TODAY[g_completingIdx]);
                g_state = STATE_QUEST_COMPLETE;
                render_quest_complete();
            }
            break;

        case STATE_QUEST_COMPLETE:
            if (padPressed & (KEY_A | KEY_B)) {
                g_state = STATE_DASHBOARD;
                render_dashboard();
            }
            break;

        case STATE_STATS:
            if (padPressed & (KEY_B | KEY_START)) {
                g_state = STATE_DASHBOARD;
                render_dashboard();
            }
            break;
    }
}
