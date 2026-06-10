#ifndef __ROOM__
#define __ROOM__
#include <allegro5/allegro.h>
#include "Spike.h"

#define TILE_BACK  0
#define TILE_WALL   1
#define TILE_FLOOR1  2
#define TILE_FLOOR2  3
#define TILE_SPIKE  4
#define TILE_EMPTY 5

#define ROOM_ROWS   30
#define ROOM_COLS   40

typedef struct {
    int tiles[30][40];
    int right_id;
    int left_id;
    int top_id;
    int bottom_id;
    spike spikes[64];
    int   spike_count;
} room;

void room_draw(room *r,float tile_w, float tile_h,ALLEGRO_BITMAP* tile_sprites[]);

void room_build_obstacles(room *r, float tile_w, float tile_h);

#endif