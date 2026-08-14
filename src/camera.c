#include "camera.h"

static u16 camX;
static u16 levelWidth;

void camera_init(u16 levelPixelWidth) {
    camX = 0;
    levelWidth = levelPixelWidth;
    bgSetScroll(0, 0, 0);
}

void camera_follow(s16 targetX) {
    s16 desired = (s16)(targetX - (SCREEN_WIDTH / 2));
    s16 maxX = (s16)(levelWidth - SCREEN_WIDTH);

    if (desired < 0) {
        desired = 0;
    }
    if (desired > maxX) {
        desired = maxX;
    }

    camX = (u16)desired;
    bgSetScroll(0, camX, 0);
}

u16 camera_get_x(void) {
    return camX;
}
