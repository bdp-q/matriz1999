#ifndef __ROOM__
#define __ROOM__
#include <allegro5/allegro.h>
#include "Danger.h"

#define TILE_BACK  0
#define TILE_WALL   1
#define TILE_FLOOR1  2
#define TILE_FLOOR2  3
#define TILE_SPIKE  4
#define TILE_EMPTY 5
#define TILE_LASER 6
#define TILE_SHOOTER 7
#define TILE_AIR_SPIKE 8
#define TILE_MOVING_SPIKE 9
#define TILE_RED_PILL 10

#define ROOM_ROWS   30
#define ROOM_COLS   40

typedef struct {
    int tiles[30][40];
    int right_id;
    int left_id;
    int top_id;
    int bottom_id;

    hitbox spikes[64];
    int  spike_count;

    air_spike air_spikes[64];
    int air_spikes_count;

    moving_spike moving_spikes[64];
    int moving_spikes_count;

    hitbox lasers[64];
    int laser_count;
    int lasers_on;
    int laser_timer;

    bullet_spawner spawners[16]; 
    int spawner_count;
    
    bullet bullets[128]; 
    int bullet_count;

} room;

void room_draw(room *r,float tile_w, float tile_h,ALLEGRO_BITMAP* tile_sprites[]);

void room_build_obstacles(room *r, float tile_w, float tile_h);

void room_update(room *r, unsigned player_x, unsigned short player_y, float tile_w, float tile_h);
#endif