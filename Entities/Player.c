#include <stdlib.h>
#include "Player.h"

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

void player_destroy(player *element){																											//Implementação da função "player_destroy"
	joystick_destroy(element->control);																											//Destrói controle do quadrado (!)
	free(element);																																//Libera a memória do quadrado na heap
}