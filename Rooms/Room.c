#include "Room.h"
#include <allegro5/allegro_primitives.h>

void room_draw(room *r,int tile_w, int tile_h) {
    for (int row = 0; row < ROOM_ROWS; row++) {
        for (int col = 0; col < ROOM_COLS; col++) {
            if (r->tiles[row][col] == TILE_EMPTY) continue;

            float x = col * tile_w;
            float y = row * tile_h;

            ALLEGRO_COLOR color = (r->tiles[row][col] == TILE_WALL)
                ? al_map_rgb(100, 100, 100)
                : al_map_rgb(50, 50, 50);
            if(r->tiles[row][col] == TILE_SPIKE)
                color = al_map_rgb(30,10,10);
            al_draw_filled_rectangle(x,y,x+tile_w,y+tile_h,color); 
        }
    }
}