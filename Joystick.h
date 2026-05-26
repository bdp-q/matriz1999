#ifndef __JOYSTICK__
#define __JOYSTICK__

typedef struct {
	unsigned char right;
	unsigned char left;
	unsigned char up;
	unsigned char down;
} joystick;

//funcao que cria o joystick
joystick* joystick_create();	
//funcao que libera ele
void joystick_destroy(joystick *element);

//funcoes que ativam andar na direita,esquerda cima e baixo
void joystick_right(joystick *element);
void joystick_left(joystick *element);
void joystick_up(joystick *element);
void joystick_down(joystick *element);

#endif