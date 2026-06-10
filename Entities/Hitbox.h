#ifndef __HITBOX__		
#define __HITBOX__

typedef struct {
    unsigned short x;			//Posição X 
	unsigned short y;			//Posição Y 
    float hw;                   // metade da largura
    float hh;                   //metade da altura
} hitbox;

//retorna 1 se tem colisão, 0 se nao tem
int hitbox_collide(hitbox *a, hitbox *b);


#endif