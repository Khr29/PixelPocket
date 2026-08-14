#ifndef CAMERA_H
#define CAMERA_H

#include <snes.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224

// Sets the scrollable world width (in pixels) the camera is clamped to.
void camera_init(u16 levelPixelWidth);

// Recomputes the camera's horizontal scroll to keep targetX near the
// screen center (clamped to the level bounds) and pushes it to BG0.
void camera_follow(s16 targetX);

// Current camera-left world x (matches the last bgSetScroll value).
u16 camera_get_x(void);

#endif // CAMERA_H
