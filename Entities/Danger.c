#include "Danger.h"

hitbox spike_build(int row, int col, float tile_w, float tile_h) {
    hitbox s;
    float hb_top = row * tile_h + tile_h * SPIKE_HITBOX_TOP; // topo da hitbox
    float hb_h   = tile_h * (1.0f - SPIKE_HITBOX_TOP);     

    s.x  = col * tile_w + tile_w * 0.5f;
    s.y  = hb_top + hb_h * 0.5f;
    s.hw = tile_w * 0.5f;
    s.hh = hb_h   * 0.5f;
    return s;
}

hitbox laser_build(int row, int col,int length, float tile_w, float tile_h) {
    hitbox l;
    float total_height = length * tile_h;

    l.x  = col * tile_w + tile_w * 0.5f;
    l.y  = row * tile_h + total_height * 0.5f;
    l.hw = tile_w * 0.2f;
    l.hh = total_height * 0.5f;
    return l;
}