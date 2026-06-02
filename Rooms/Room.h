#ifndef __ROOM__
#define __ROOM__

#define TILE_EMPTY  0
#define TILE_WALL   1
#define TILE_FLOOR  2
#define TILE_SPIKE  3

#define ROOM_ROWS   10
#define ROOM_COLS   16
#define TILE_SIZE   64


typedef struct {
    int tiles[10][16];
    struct room * right; //tem que fazer isso
} room;

void room_draw(room *r,int tile_w, int tile_h);


#endif