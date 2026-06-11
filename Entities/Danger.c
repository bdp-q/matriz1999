#include <math.h>
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

bullet bullet_fire(bullet_spawner *s, unsigned short target_x, unsigned short target_y, float speed) {
    bullet b;
    b.x  = s->x;
    b.y  = s->y;
    b.hw = 4.0f;   
    b.hh = 4.0f;
    b.active = 1;

    // direção normalizada do spawner até o player
    float dx = target_x - s->x;
    float dy = target_y - s->y;
    float dist = sqrtf(dx*dx + dy*dy);
    if (dist == 0) dist = 1;  // evita divisão por zero

    b.vx = (dx / dist) * speed;
    b.vy = (dy / dist) * speed;
    return b;
}

void bullet_update(bullet *b) {
    if (!b->active) return;
    b->x += b->vx;
    b->y += b->vy;
}