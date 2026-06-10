#include "Spike.h"

spike spike_build(int row, int col, float tile_w, float tile_h) {
    spike s;
    float hb_top = row * tile_h + tile_h * SPIKE_HITBOX_TOP; // topo da hitbox
    float hb_h   = tile_h * (1.0f - SPIKE_HITBOX_TOP);     

    s.hb.x  = col * tile_w + tile_w * 0.5f;
    s.hb.y  = hb_top + hb_h * 0.5f;
    s.hb.hw = tile_w * 0.5f;
    s.hb.hh = hb_h   * 0.5f;
    return s;
}