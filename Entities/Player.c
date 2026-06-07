#include <stdlib.h>
#include "Player.h"
#include "Rooms/Room.h"
#include<stdio.h>
#define GRAVITY 1.0f
#define MAX_FALL 12.0f
#define IDLE 0
#define CORRENDO 1

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
static inline int tile_has_collision(int tile) {
    return tile == TILE_WALL || tile == TILE_FLOOR1 || tile == TILE_FLOOR2;
}

int colision_left(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int col      = (int)(p->x - p->side/2) / tile_w;
    int row_top  = (int)(p->y - p->side/2) / tile_h;
    int row_bot  = (int)(p->y + p->side/2 - 1) / tile_h;

    return tile_has_collision(r->tiles[row_top][col]) ||
           tile_has_collision(r->tiles[row_bot][col]);
}

int colision_right(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int col      = (int)(p->x + p->side/2 - 1) / tile_w;
    int row_top  = (int)(p->y - p->side/2) / tile_h;
    int row_bot  = (int)(p->y + p->side/2 - 1) / tile_h;
    
    
    return tile_has_collision(r->tiles[row_top][col]) ||
           tile_has_collision(r->tiles[row_bot][col]);
}

int colision_bottom(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int row      = (int)(p->y + p->side/2) / tile_h;
    int col_left = (int)(p->x - p->side/2) / tile_w;
    int col_right= (int)(p->x + p->side/2 - 1) / tile_w;

    return tile_has_collision(r->tiles[row][col_left]) ||
           tile_has_collision(r->tiles[row][col_right]);
}

int colision_top(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int row      = (int)(p->y - p->side/2) / tile_h;
    int col_left = (int)(p->x - p->side/2) / tile_w;
    int col_right= (int)(p->x + p->side/2 - 1) / tile_w;

    return tile_has_collision(r->tiles[row][col_left]) ||
           tile_has_collision(r->tiles[row][col_right]);
}

void player_update(player *p, room rooms[], unsigned short max_x, unsigned short max_y){

    if (p->control->left){
        p->direcao=0;
        p->anim_state=CORRENDO;
        player_move(p, 1, 0, max_x, max_y);
        if(colision_left(p,&rooms[p->room_id],max_x,max_y))
            p->x = ((int)(p->x - p->side/2) / (max_x/ROOM_COLS) + 1) * (max_x/ROOM_COLS) + p->side/2;
    }
    if (p->control->right){
        p->direcao=1;
        p->anim_state=CORRENDO;
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

    if (p->x - p->side/2 <= 0){
        p->room_id = rooms[p->room_id].left_id;
        p->x = max_x - p->side;
    }

    if (p->x + p->side/2 >= max_x){ //vai para a direita
        p->room_id = rooms[p->room_id].right_id;
        p->x = p->side;
    }
    //terminar bomba de animação
    p->anim_timer++;
    if (p->anim_timer >= 15) {
        p->anim_timer = 0;
        p->anim_frame = (p->anim_frame + 1) % 2;
    }

}

void player_destroy(player *p){																													//Implementação da função "player_destroy"
	joystick_destroy(p->control);																											//Destrói controle do quadrado (!)
	free(p);																																//Libera a memória do quadrado na heap
}