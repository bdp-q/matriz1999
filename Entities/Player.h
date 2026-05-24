#ifndef __PLAYER__		
#define __PLAYER__

#include "Joystick.h"			//onde fica o movimento do jogador																										

#define PLAYER_STEP 10			//Tamanho, em pixels, de um passo do jogador (movimento)

typedef struct {		
	unsigned char side;			//Tamanmho da lateral da hitbox do jogador
	unsigned short x;			//Posição X 
	unsigned short y;			//Posição Y do centro do jogador
	unsigned char hp;
	joystick *control;			// movimento dele																						

} player;						//Struct do jogador

// função que cria e retorna o jogador
player* player_create(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char hp);
// função que atualiza o jogador
//void player_move(player *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);			//Protótipo da função de movimentação do jogador
void player_destroy(player *element);																							//Protótipo da função de destruição do jogador

#endif