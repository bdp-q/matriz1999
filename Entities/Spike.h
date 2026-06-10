#ifndef __SPIKE__
#define __SPIKE__

#include "Hitbox.h"

#define SPIKE_HITBOX_TOP 0.2f

typedef struct {
    hitbox hb;
} spike;

spike spike_build(int row, int col, float tile_w, float tile_h);

#endif