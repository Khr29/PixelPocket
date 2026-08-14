#ifndef INPUT_H
#define INPUT_H

#include <snes.h>

// Thin wrapper around PVSnesLib's pad reader. Call input_update() once per
// frame before anything queries held()/pressed().
void input_update(void);

// Buttons currently held down.
u16 input_held(void);

// Buttons newly pressed this frame (edge-triggered).
u16 input_pressed(void);

#endif // INPUT_H
