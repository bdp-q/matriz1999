#include "Room.h"
#include "Danger.h"
#include <allegro5/allegro_primitives.h>

void room_draw(room *r,float tile_w, float tile_h, ALLEGRO_BITMAP* tile_sprites[]) {
    for (int row = 0; row < ROOM_ROWS; row++) {
        for (int col = 0; col < ROOM_COLS; col++) {
            int tipo = r->tiles[row][col];

            float x = col * tile_w;
            float y = row * tile_h;

            ALLEGRO_BITMAP* sprite = tile_sprites[tipo];

        /**/
            if (tile_sprites[TILE_BACK]) {
                al_draw_scaled_bitmap(tile_sprites[TILE_BACK],
                    0, 0,
                    al_get_bitmap_width(tile_sprites[TILE_BACK]),
                    al_get_bitmap_height(tile_sprites[TILE_BACK]),
                    x, y, tile_w, tile_h, 0);
            }
            if (sprite) {
                // tem sprite na gaveta: desenha ele escalado pro tamanho do tile
                al_draw_scaled_bitmap(sprite,
                    0, 0,
                    al_get_bitmap_width(sprite),
                    al_get_bitmap_height(sprite),
                    x, y, tile_w, tile_h,
                    0);
            } 
            else {
                // gaveta sem sprite: desenha retângulo colorido (útil pra debug)
                al_draw_filled_rectangle(x, y, x+tile_w, y+tile_h, al_map_rgb(0, 0, 255));
            }
        }
    }
    if (r->lasers_on) {
        for(int i = 0; i < r->laser_count; i++) {
            hitbox l = r->lasers[i];
            al_draw_filled_rectangle(l.x - l.hw, l.y - l.hh, l.x + l.hw, l.y + l.hh, al_map_rgb(255, 0, 0));
        }
    }

}

void room_build_obstacles(room *r, float tile_w, float tile_h) {
    r->spike_count = 0;
    r->laser_count = 0;
    r->laser_timer = 0;
    r->lasers_on = 1;

    for (int row = 0; row < ROOM_ROWS; row++) {
        for (int col = 0; col < ROOM_COLS; col++) {
            if (r->tiles[row][col] == TILE_SPIKE)
                r->spikes[r->spike_count++] = spike_build(row, col, tile_w, tile_h);
            if(r->tiles[row][col] == TILE_LASER){
                int end_row = row + 1; // checa se bateu numa parede
                while(end_row < ROOM_ROWS && 
                      r->tiles[end_row][col] != TILE_WALL && 
                      r->tiles[end_row][col] != TILE_FLOOR1 && 
                      r->tiles[end_row][col] != TILE_FLOOR2) {
                    end_row++;
                }
                int length = end_row - row;
                r->lasers[r->laser_count++] = laser_build(row, col, length, tile_w, tile_h);  
            }
        }
    }
}