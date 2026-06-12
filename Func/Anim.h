#ifndef __ANIM__		
#define __ANIM__

typedef struct {
    int frame;        // frame atual
    int timer;        // contador de frames desde a última troca
    int velocity;        // quantos frames de jogo por frame de sprite
    int frame_count;  // total de frames na spritesheet
    int state;         //qual o estado a animação está (só pro player)
} anim;

//avanca a animacao
void anim_update(anim *a);

#endif