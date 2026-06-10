#include <stdlib.h>
#include "Player.h"
#include "Hitbox.h"
#include "Rooms/Room.h"
#include<stdio.h>
#define GRAVITY 1.0f
#define MAX_FALL 12.0f


player* player_create(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char hp){	
	
	//verifica se ele nao esta sendo iniciado fora da tela
	if ((x - side/2 < 0) || (x + side/2 > max_x) || (y - side/2 < 0) || (y + side/2 > max_y)) 
		return NULL;	

	// cria/inicializa o jogador
	player *new_player = (player*) malloc(sizeof(player));																				
	new_player->side = side;																												
	new_player->x = x;																												
	new_player->y = y;																													
	new_player->hp = hp;
	new_player->control = joystick_create();
    new_player->room_id = 0;
    new_player->anim_frame=0;
    new_player->anim_timer=0;
    new_player->direcao=1;	
    new_player->anim_state=IDLE;	
    new_player->anim_velocity= 16;																					
	return new_player;																															
}

void player_move(player *p, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y){
	//verifica se é possível e dai vai se for!
	if (!trajectory){
		p->x = p->x - steps*PLAYER_STEP; //esquerda
	} 				
	else if (trajectory == 1){
		p->x = p->x + steps*PLAYER_STEP; //direita
	}
}

void check_spike_damage(player *p, room *r) {
    hitbox ph = { p->x, p->y, p->side * 0.5f, p->side * 0.5f };
    for (int i = 0; i < r->spike_count; i++) {
        if (hitbox_collide(&ph, &r->spikes[i].hb)) {
            p->is_damaged = 1;
            break;
        }
    }
}

int tile_has_collision(player *p, int tile) {  
    return  tile == TILE_WALL || tile == TILE_FLOOR1 || tile == TILE_FLOOR2;
}

int colision_left(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int col      = (int)(p->x - p->side/2) / tile_w;
    int row_top  = (int)(p->y - p->side/2) / tile_h;
    int row_bot  = (int)(p->y + p->side/2 - 1) / tile_h;

    return tile_has_collision(p,r->tiles[row_top][col]) ||
           tile_has_collision(p,r->tiles[row_bot][col]);
}

int colision_right(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int col      = (int)(p->x + p->side/2 - 1) / tile_w;
    int row_top  = (int)(p->y - p->side/2) / tile_h;
    int row_bot  = (int)(p->y + p->side/2 - 1) / tile_h;
    
    
    return tile_has_collision(p,r->tiles[row_top][col]) ||
           tile_has_collision(p,r->tiles[row_bot][col]);
}

int colision_bottom(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int row      = (int)(p->y + p->side/2) / tile_h;
    int col_left = (int)(p->x - p->side/2) / tile_w;
    int col_right= (int)(p->x + p->side/2 - 1) / tile_w;

    return tile_has_collision(p,r->tiles[row][col_left]) ||
           tile_has_collision(p,r->tiles[row][col_right]);
}

int colision_top(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int row      = (int)(p->y - p->side/2) / tile_h;
    int col_left = (int)(p->x - p->side/2) / tile_w;
    int col_right= (int)(p->x + p->side/2 - 1) / tile_w;
    
    return tile_has_collision(p,r->tiles[row][col_left]) ||
           tile_has_collision(p,r->tiles[row][col_right]);
}

static int anim_num_frames(int state){
    switch(state) {
        case IDLE: return 2;
        case CORRENDO: return 6; 
        case PULANDO: return 4;
        case DOWN: return 4;
//        case ACAO: return ?;
    }
}

void player_update(player *p, room rooms[], unsigned short max_x, unsigned short max_y){
    int prev_state = p->anim_state;
    
    check_spike_damage(p,&rooms[p->room_id]);
    if(!p->control->down || !p->is_down){
        if (p->control->left){
            p->direcao=0;
            player_move(p, 1, 0, max_x, max_y);
            if(colision_left(p,&rooms[p->room_id],max_x,max_y))
                p->x = ((int)(p->x - p->side/2) / (max_x/ROOM_COLS) + 1) * (max_x/ROOM_COLS) + p->side/2;
        }
        if (p->control->right){
            p->direcao=1;
            player_move(p, 1, 1, max_x, max_y);
            if(colision_right(p,&rooms[p->room_id],max_x,max_y))
                p->x = ((int)(p->x + p->side/2 - 1) / (max_x/ROOM_COLS)) * (max_x/ROOM_COLS) - p->side/2;
        }

        if(p->control->up && p->is_down){
            p->gravity = -12.0f;
            p->is_down = 0;
        }

        p->gravity += GRAVITY;
        if(p->gravity > MAX_FALL) 
            p->gravity = MAX_FALL;
        p->y += p->gravity;
        
        if(colision_top(p,&rooms[p->room_id],max_x,max_y)){
            p->y = ((int)(p->y - p->side/2) / (max_y/ROOM_ROWS) + 1) * (max_y/ROOM_ROWS) + p->side/2;
            p->gravity = 3.0f;
        }

        if(colision_bottom(p,&rooms[p->room_id],max_x,max_y)){
            p->y = ((int)(p->y + p->side/2) / (max_y/ROOM_ROWS)) * (max_y/ROOM_ROWS) - p->side/2;
            p->gravity = 0;
            p->is_down = 1;
        } else {
            p->is_down = 0;
        }

        if (p->is_damaged){
            p->x = 100;
            p->y = 100;
            p->is_damaged = 0;
        }
        if (p->x - p->side/2 <= 0){
            p->room_id = rooms[p->room_id].left_id;
            p->x = max_x - p->side;
        }

        if (p->x + p->side/2 >= max_x){ //vai para a direita
            p->room_id = rooms[p->room_id].right_id;
            p->x = p->side;
        }
    }

    if (!p->is_down){
        p->anim_state = PULANDO;
        p->anim_velocity = 7;
        if(p->gravity > 0 ){
        if (p->anim_frame >= anim_num_frames(PULANDO) - 3)
            return;
        }
    }

    else if (p->control->down){
		p->anim_state= DOWN;
        p->anim_velocity=10;
        if (p->anim_frame >= anim_num_frames(DOWN) - 1){
            p->anim_frame = anim_num_frames(DOWN) - 1;
            return;
        }
    }
    
    else if (!p->control->left && !p->control->right){
        p->anim_state = IDLE;
        p->anim_velocity=25;
    }
    else{
        p->anim_state = CORRENDO;
        p->anim_velocity = 5;
    } 


    if (prev_state != p->anim_state){
        p->anim_timer = 0;
        p->anim_frame = 0;
    }   


    p->anim_timer++;
    if (p->anim_timer >= p->anim_velocity) { // velocidade da anim
        p->anim_timer = 0;
        p->anim_frame = (p->anim_frame+1) % anim_num_frames(p->anim_state);
    }

}

void player_destroy(player *p){																													//Implementação da função "player_destroy"
	joystick_destroy(p->control);																											//Destrói controle do quadrado (!)
	free(p);																																//Libera a memória do quadrado na heap
}