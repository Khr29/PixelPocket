#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <snes.h>

typedef enum {
    DIALOGUE_NPC,
    DIALOGUE_BOARD,
} DialogueKind;

// Loads the given dialogue box overlay onto BG1 and shows it. The box art
// (frame + hand-baked pixel text, see assets/ui/dialogue) is a single
// pre-rendered picture -- no runtime text engine needed for this
// prototype's handful of fixed lines.
void dialogue_show(DialogueKind kind);

// Hides BG1 again, revealing the town underneath.
void dialogue_hide(void);

bool dialogue_is_open(void);

#endif // DIALOGUE_H
