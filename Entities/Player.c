#include <stdlib.h>
#include "Player.h"
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
	return new_player;																															
}

void player_move(player *p, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y){
	//verifica se é possível e dai vai se for!
	if (!trajectory){
		if ((p->x - steps*PLAYER_STEP) - p->side/2 >= 0) 
			p->x = p->x - steps*PLAYER_STEP; //esquerda
	} 				
	else if (trajectory == 1){
		if ((p->x + steps*PLAYER_STEP) + p->side/2 <= max_x)
			p->x = p->x + steps*PLAYER_STEP; //direita
	}
}

int colision_left(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int col      = (int)(p->x - p->side/2) / tile_w;
    int row_top  = (int)(p->y - p->side/2) / tile_h;
    int row_bot  = (int)(p->y + p->side/2 - 1) / tile_h;

    return r->tiles[row_top][col] != TILE_EMPTY ||
           r->tiles[row_bot][col] != TILE_EMPTY;
}

int colision_right(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int col      = (int)(p->x + p->side/2 - 1) / tile_w;
    int row_top  = (int)(p->y - p->side/2) / tile_h;
    int row_bot  = (int)(p->y + p->side/2 - 1) / tile_h;

    return r->tiles[row_top][col] != TILE_EMPTY ||
           r->tiles[row_bot][col] != TILE_EMPTY;
}

int colision_bottom(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int row      = (int)(p->y + p->side/2) / tile_h;
    int col_left = (int)(p->x - p->side/2) / tile_w;
    int col_right= (int)(p->x + p->side/2 - 1) / tile_w;

    return r->tiles[row][col_left] != TILE_EMPTY ||
           r->tiles[row][col_right]!= TILE_EMPTY;
}

int colision_top(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int row      = (int)(p->y - p->side/2) / tile_h;
    int col_left = (int)(p->x - p->side/2) / tile_w;
    int col_right= (int)(p->x + p->side/2 - 1) / tile_w;

    return r->tiles[row][col_left] != TILE_EMPTY ||
           r->tiles[row][col_right]!= TILE_EMPTY;
}

void player_update(player *p, room *r, unsigned short max_x, unsigned short max_y){

    if (p->control->left){
        player_move(p, 1, 0, max_x, max_y);
        if(colision_left(p,r,max_x,max_y))
            p->x = ((int)(p->x - p->side/2) / (max_x/ROOM_COLS) + 1) * (max_x/ROOM_COLS) + p->side/2;
    }
    if (p->control->right){
        player_move(p, 1, 1, max_x, max_y);
        if(colision_right(p,r,max_x,max_y))
            p->x = ((int)(p->x + p->side/2 - 1) / (max_x/ROOM_COLS)) * (max_x/ROOM_COLS) - p->side/2;
    }

    if(p->control->up && p->is_down){
        p->gravity = -18.0f;
        p->is_down = 0;
    }

    p->gravity += GRAVITY;
    if(p->gravity > MAX_FALL) 
		p->gravity = MAX_FALL;
	p->y += p->gravity;
    
	if(colision_top(p,r,max_x,max_y)){
		p->y = ((int)(p->y - p->side/2) / (max_y/ROOM_ROWS) + 1) * (max_y/ROOM_ROWS) + p->side/2;
    	p->gravity = 0;
    }

	if(colision_bottom(p,r,max_x,max_y)){
        p->y = ((int)(p->y + p->side/2) / (max_y/ROOM_ROWS)) * (max_y/ROOM_ROWS) - p->side/2;
        p->gravity = 0;
        p->is_down = 1;
    } else {
        p->is_down = 0;
    }
	
}

void player_destroy(player *p){																													//Implementação da função "player_destroy"
	joystick_destroy(p->control);																											//Destrói controle do quadrado (!)
	free(p);																																//Libera a memória do quadrado na heap
}