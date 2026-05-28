#include <stdlib.h>
#include "Player.h"
#include "Rooms/Room.h"
#include<stdio.h>
#define GRAVITY 0.9f
#define MAX_FALL 10.0f

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

int colision_bottom(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
	int pos_tile_col =  (int) (p->x + p->side/2) / (x_screen / ROOM_COLS);
	int pos_tile_row = (int) (p->y + p->side/2) / (y_screen / ROOM_ROWS);
	if (r->tiles[pos_tile_row][pos_tile_col] == TILE_FLOOR)
		return 1;
	return 0;
}

int colision_left(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
	int pos_tile_col =  (int) (p->x - p->side/2) / (x_screen / ROOM_COLS);
	int pos_tile_row = (int) (p->y + p->side/2) / (y_screen / ROOM_ROWS);
	if (r->tiles[pos_tile_row][pos_tile_col] == TILE_WALL)
		return 1;
	return 0;
}

void player_update(player *p, room *r, unsigned short max_x, unsigned short max_y){
	
	
	if (p->control->left){				 //altera a posição do jogador pra esq																																				
		player_move(p, 1, 0, max_x, max_y);																																				
		if(colision_left(p,r,max_x,max_y))
			player_move(p,1,1,max_x,max_y);														
	}
	if (p->control->right){				//altera a posição do jogador pra dir
		player_move(p, 1, 1, max_x, max_y);	
	}


	p->gravity += GRAVITY;

	if(p->gravity > MAX_FALL)
		p->gravity = MAX_FALL;
	p->y += (short)p->gravity;
	
	if(colision_bottom(p,r,max_x,max_y)){		//verifica se o jogador esta no chão
		p->y = ((p->y + p->side/2) / (max_y / ROOM_ROWS)) * (max_y / ROOM_ROWS) - p->side/2;
		p->gravity = 0;
		p->is_down = 1;
	}
	else{
		p->is_down = 0;
	}

	if(p->control->up && p->is_down){		//faz o jogador pular se ele estiver no chão
		p->gravity = -18.0f;
		fprintf(stderr,"ENTREI");
		p->is_down = 0;
		return;
	}



	return;
}

void player_destroy(player *p){																													//Implementação da função "player_destroy"
	joystick_destroy(p->control);																											//Destrói controle do quadrado (!)
	free(p);																																//Libera a memória do quadrado na heap
}