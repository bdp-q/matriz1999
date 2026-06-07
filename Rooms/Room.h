#ifndef __ROOM__
#define __ROOM__

#define TILE_EMPTY  0
#define TILE_WALL   1
#define TILE_FLOOR  2
#define TILE_SPIKE  3

#define ROOM_ROWS   33
#define ROOM_COLS   60

typedef struct {
    int tiles[33][60];
    int right_id;
    int left_id;
    int top_id;
    int bottom_id;
} room;

void room_draw(room *r,int tile_w, int tile_h);


#endif