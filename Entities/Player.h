#ifndef __PLAYER__		
#define __PLAYER__

#include "Joystick.h"			//onde fica o movimento do jogador																										
#include "Rooms/Room.h"
#define PLAYER_STEP 7			//Tamanho, em pixels, de um passo do jogador (movimento)
#define IDLE 0
#define CORRENDO 1
#define PULANDO 2
#define DOWN 3

typedef struct {		
	unsigned char side;			//Tamanmho da lateral da hitbox do jogador
	unsigned short x;			//Posição X 
	unsigned short y;			//Posição Y do centro do jogador
	unsigned char hp;
	char is_down;				//diz se o jogador esta tocando em algo no chão
	float gravity;				//velocidade puxando ele pra baixo
	joystick *control;			// movimento dele fica nessa struct	
	int room_id;				//id do quarto que o player atualmente esta		
	
	int direcao;				//direção que o jogador esta olhando: 1 direita 0 esquerda
	int anim_frame;				//em que ponto da animacao estamos
	int anim_timer;				// a quanto tempo ela ta la
	int anim_state;				//qual animação ele esta
	int anim_velocity;			//qual a velocidade dessa animação

} player;						//Struct do jogador

// função que cria e retorna o jogador
player* player_create(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char hp);

// função que atualiza o jogador
void player_move(player *p, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);

//função que atualiza o jogador
void player_update(player *p,room *r, unsigned short max_x, unsigned short max_y);

//função que libera a memória do jogador na hora do programa encerrar
void player_destroy(player *p);

#endif
