#include "Room.h"
#include "Danger.h"
#include <allegro5/allegro_primitives.h>

void room_draw(room *r,float tile_w, float tile_h, ALLEGRO_BITMAP* tile_sprites[]) {
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

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
            if((sprite == tile_sprites[TILE_AIR_SPIKE]) ||sprite == tile_sprites[TILE_MOVING_SPIKE]) continue;

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

            al_draw_filled_rectangle(l.x - l.hw, l.y - l.hh, l.x + l.hw, l.y + l.hh, al_premul_rgba_f(0.0, 1.0, 0.0, 0.7) );
        }
    }
    for (int i = 0; i < r->bullet_count; i++) {
        bullet *b = &r->bullets[i];
        if (b->active)
            al_draw_filled_circle(b->x, b->y, b->hw, al_map_rgb(0, 255, 65));
    }
    for (int i = 0; i < r->air_spikes_count; i++) {
        air_spike *as = &r->air_spikes[i];
        al_draw_scaled_bitmap(
        tile_sprites[TILE_AIR_SPIKE],
        as->anim.frame * 48, 0,      // recorte: pula N frames pra direita
        48, 48,           // tamanho do recorte na spritesheet
        as->hb.x - as->hb.hw, as->hb.y -  as->hb.hh,              // posição na tela
        tile_w, tile_h,             // tamanho na tela
        0
        );
    }
    for (int i = 0; i < r->moving_spikes_count; i++) {
        moving_spike *ms = &r->moving_spikes[i];
        int flip = (ms->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;

        al_draw_scaled_bitmap(
        tile_sprites[TILE_MOVING_SPIKE],
        ms->anim.frame * 48, 0,      // recorte: pula N frames pra direita
        48, 48,           // tamanho do recorte na spritesheet
        ms->x - ms->hb.hw, ms->y -  ms->hb.hh,              // posição na tela
        tile_w, tile_h,             // tamanho na tela
        flip
        );
    }
}

void room_build_obstacles(room *r, float tile_w, float tile_h) {
    r->spike_count = 0;
    r->laser_count = 0;
    r->laser_timer = 0;
    r->lasers_on = 0;
    r->spawner_count = 0;
    r->bullet_count  = 0;
    r->air_spikes_count = 0;
    r->moving_spikes_count = 0;

    for (int row = 0; row < ROOM_ROWS; row++) {
        for (int col = 0; col < ROOM_COLS; col++) {
            if (r->tiles[row][col] == TILE_SPIKE)
                r->spikes[r->spike_count++] = spike_build(row, col, tile_w, tile_h);
            
            else if(r->tiles[row][col] == TILE_AIR_SPIKE)
                r->air_spikes[r->air_spikes_count++] = air_spike_build(row, col, tile_w, tile_h);

            else if(r->tiles[row][col] == TILE_MOVING_SPIKE)
                r->moving_spikes[r->moving_spikes_count++] = moving_spike_build(row, col, tile_w, tile_h);

            else if(r->tiles[row][col] == TILE_LASER){
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
            else if (r->tiles[row][col] == TILE_SHOOTER) {
                bullet_spawner sp; // criar funçao "spawner_build"
                sp.x        = col * tile_w + tile_w * 0.5f;
                sp.y        = row * tile_h + tile_h * 0.5f;
                sp.timer    = 0;
                sp.interval = 90;   // atira a cada 3 segundos (90 frames a 30fps)
                r->spawners[r->spawner_count++] = sp;
            }
        }
    }
}

void room_update(room *r, unsigned player_x, unsigned short player_y, float tile_w, float tile_h, int is_down){
    float screen_w = tile_w * ROOM_COLS;
    float screen_h = tile_h * ROOM_ROWS;

     for (int i = 0; i < r->spawner_count; i++) {
        bullet_spawner *s = &r->spawners[i];
        s->timer++;
        if (s->timer >= s->interval) {
            s->timer = 0;
            // procura um slot livre (active == 0) ou expande
            int slot = -1;
            for (int j = 0; j < r->bullet_count; j++) {
                if (!r->bullets[j].active){
                    slot = j;
                    break;
                } 
            }
            if (slot == -1 && r->bullet_count < 128)
                slot = r->bullet_count++;
            if (slot != -1)
                r->bullets[slot] = bullet_build(s, player_x, player_y, 4.0f);
               
        }
    }
    for (int i = 0; i < r->bullet_count; i++) {
        bullet *b = &r->bullets[i];
        if (b->active){
            bullet_update(b,is_down);
            if (b->x < 0 || b->x > screen_w || b->y < 0 || b->y > screen_h)
                b->active = 0;
        }
    } 

    r->laser_timer++;
    if (r->laser_timer >= 60) {
        r->laser_timer = 0;
        r->lasers_on = !r->lasers_on;
    }

    for (int i = 0; i < r->air_spikes_count; i++) {
        anim_update(&r->air_spikes[i].anim);
    }
    for (int i = 0; i < r->moving_spikes_count; i++) {
        moving_spike *ms = &r->moving_spikes[i];
        anim_update(&ms->anim);

        ms->x += ms->direcao * ms->vx;
        ms->moved += ms->vx;

        if (ms->moved >= ms->range) {
            ms->moved = 0.0f;
            ms->direcao  *= -1;
        }
        ms->hb.x = (unsigned short)ms->x;
    }
}
