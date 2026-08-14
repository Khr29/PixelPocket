#include "input.h"

static u16 heldButtons;
static u16 pressedButtons;

void input_update(void) {
    heldButtons = padsCurrent(0);
    pressedButtons = padsDown(0);
}

u16 input_held(void) {
    return heldButtons;
}

u16 input_pressed(void) {
    return pressedButtons;
}
