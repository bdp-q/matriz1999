#ifndef __DANGER__
#define __DANGER__

#include "Hitbox.h"
#include "Func/Anim.h"

#define SPIKE_HITBOX_TOP 0.2f

typedef struct {
    unsigned short x;
    unsigned short y;     // posição do centro
    float hw;
    float hh;   // meia largura/altura (hitbox)
    float vx;
    float vy;   // velocidade por frame
    int   active;   // 1 = existe na tela, 0 = morto
} bullet;

typedef struct {
    unsigned short x;
    unsigned short y;         // posição fixa do spawner (em pixels)
    int   timer;        // contador de frames desde o último tiro
    int   interval;     // quantos frames entre cada tiro
} bullet_spawner;

typedef struct {
    anim anim;
    hitbox hb;
} air_spike;

typedef struct {
    anim anim;
    hitbox hb;
    float x;     
    float y;
    float vx;           
    int   direcao;          // 1 = direita, -1 = esquerda
    float moved;            //quanto moveu
    float range;            //quanto precisa pra mudar de direcao
} moving_spike;
hitbox spike_build(int row, int col, float tile_w, float tile_h);

air_spike air_spike_build(int row, int col, float tile_w, float tile_h);

moving_spike moving_spike_build(int row, int col, float tile_w, float tile_h);

hitbox laser_build(int row, int col, int length, float tile_w, float tile_h);

bullet bullet_build(bullet_spawner *s, unsigned short target_x, unsigned short target_y, float speed);

void bullet_update(bullet *b);

#endif