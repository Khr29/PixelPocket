#ifndef QUEST_DATA_H
#define QUEST_DATA_H

#include <snes.h>

// Quest data model, kept separate from rendering (dashboard.c/text.c) and
// game logic (quest.c). An eventual external AI quest generator would
// target this same shape: id/name/category/description/difficulty/xp/
// statReward. QUEST_TODAY/QUEST_SIDE below are hand-written sample data
// standing in for that until the generator exists.

typedef enum {
    QUEST_CAT_READING,
    QUEST_CAT_LEARNING,
    QUEST_CAT_CODING,
    QUEST_CAT_CREATIVE,
    QUEST_CAT_LIFE,
    QUEST_CAT_EXPLORATION,
} QuestCategory;

typedef struct {
    u8 id;
    const char *name;
    QuestCategory category;
    const char *description; // kept short -- rendered on one line
    u8 difficulty;            // 1-5 stars
    u16 xp;
    bool completed;
} Quest;

#define QUEST_TODAY_COUNT 4
#define QUEST_SIDE_COUNT 3

static Quest QUEST_TODAY[QUEST_TODAY_COUNT] = {
    {1, "READ 10 PAGES", QUEST_CAT_READING, "READ ANY BOOK OR ARTICLE.", 2, 40, true},
    {2, "STUDY 30 MIN", QUEST_CAT_LEARNING, "FOCUSED STUDY SESSION.", 3, 80, true},
    {3, "PRACTICE CODING", QUEST_CAT_CODING, "WORK ON A CODE PROJECT.", 3, 100, false},
    {4, "CLEAN YOUR DESK", QUEST_CAT_LIFE, "TIDY YOUR WORKSPACE.", 1, 20, false},
};

static Quest QUEST_SIDE[QUEST_SIDE_COUNT] = {
    {5, "LEARN SOMETHING NEW", QUEST_CAT_LEARNING, "ONE NEW FACT OR SKILL.", 2, 50, false},
    {6, "WRITE 100 WORDS", QUEST_CAT_CREATIVE, "WRITE ANYTHING AT ALL.", 2, 40, false},
    {7, "TAKE 5 PHOTOGRAPHS", QUEST_CAT_EXPLORATION, "FIND 5 INTERESTING SHOTS.", 1, 30, false},
};

static const char *quest_category_name(QuestCategory c) {
    switch (c) {
        case QUEST_CAT_READING: return "READING";
        case QUEST_CAT_LEARNING: return "LEARNING";
        case QUEST_CAT_CODING: return "CODING";
        case QUEST_CAT_CREATIVE: return "CREATIVE";
        case QUEST_CAT_LIFE: return "LIFE";
        case QUEST_CAT_EXPLORATION: return "EXPLORATION";
        default: return "";
    }
}

#endif // QUEST_DATA_H
