#ifndef __DANGER__
#define __DANGER__

#include "Hitbox.h"

#define SPIKE_HITBOX_TOP 0.2f


hitbox spike_build(int row, int col, float tile_w, float tile_h);

hitbox laser_build(int row, int col, int length, float tile_w, float tile_h);
#endif