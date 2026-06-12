#include <stdlib.h>
#include "Player.h"
#include "Hitbox.h"
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
    new_player->room_id = 0;
    new_player->anim.frame=0;
    new_player->anim.timer=0;
    new_player->direcao=1;	
    new_player->anim.state=IDLE;	
    new_player->anim.velocity= 16;	
    new_player->in_action = 0;																				
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

void check_damage(player *p, room *r){
    hitbox ph = { p->x, p->y, p->side * 0.5f, p->side * 0.5f };

    //checa espinho
    for (int i = 0; i < r->spike_count; i++) {
        if (hitbox_collide(&ph, &r->spikes[i])) {
            p->is_damaged = 1;
            break;
        }
    }
    //checa o laser
    for (int i = 0; i < r->laser_count; i++) {
        if (hitbox_collide(&ph, &r->lasers[i]) && r->lasers_on) {
            p->is_damaged = 1;
            break;
        }
    }
    //checa o spike air
    for (int i = 0; i < r->air_spikes_count; i++) {
        if (hitbox_collide(&ph, &r->air_spikes[i].hb)) {
            p->is_damaged = 1;
            break;
        }
    }
    //checa as balas
    for (int i = 0; i < r->bullet_count; i++) {
        bullet *b = &r->bullets[i];
        if (b->active){
            hitbox bh = {b->x,b->y,b->hw, b->hh};
            if (hitbox_collide(&ph, &bh)) {
                p->is_damaged = 1;
                b->active = 0;  // tiro some ao acertar
                break;
            }
        }
    }

    //checa o moving spike
    for (int i = 0; i < r->moving_spikes_count; i++) {
        if (hitbox_collide(&ph, &r->moving_spikes[i].hb)) {
            p->is_damaged = 1;
            break;
        }
    }
}

int tile_has_collision(player *p, int tile) {  
    return  tile == TILE_WALL || tile == TILE_FLOOR1 || tile == TILE_SHOOTER;
}

int colision_left(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int col      = (int)(p->x - p->side/2) / tile_w;
    int row_top  = (int)(p->y - p->side/2) / tile_h;
    int row_bot  = (int)(p->y + p->side/2 - 1) / tile_h;

    return tile_has_collision(p,r->tiles[row_top][col]) ||
           tile_has_collision(p,r->tiles[row_bot][col]);
}

int colision_right(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int col      = (int)(p->x + p->side/2 - 1) / tile_w;
    int row_top  = (int)(p->y - p->side/2) / tile_h;
    int row_bot  = (int)(p->y + p->side/2 - 1) / tile_h;
    
    
    return tile_has_collision(p,r->tiles[row_top][col]) ||
           tile_has_collision(p,r->tiles[row_bot][col]);
}

int colision_bottom(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int row      = (int)(p->y + p->side/2) / tile_h;
    int col_left = (int)(p->x - p->side/2) / tile_w;
    int col_right= (int)(p->x + p->side/2 - 1) / tile_w;

    return tile_has_collision(p,r->tiles[row][col_left]) ||
           tile_has_collision(p,r->tiles[row][col_right]);
}

int colision_top(player *p, room *r, unsigned short x_screen, unsigned short y_screen){
    int tile_w = x_screen / ROOM_COLS;
    int tile_h = y_screen / ROOM_ROWS;
    int row      = (int)(p->y - p->side/2) / tile_h;
    int col_left = (int)(p->x - p->side/2) / tile_w;
    int col_right= (int)(p->x + p->side/2 - 1) / tile_w;
    
    return tile_has_collision(p,r->tiles[row][col_left]) ||
           tile_has_collision(p,r->tiles[row][col_right]);
}

static int anim_num_frames(int state){
    switch(state) {
        case IDLE: return 2;
        case CORRENDO: return 6; 
        case PULANDO: return 4;
        case DOWN: return 4;
        case ACAO: return 11;
        default: return 1;
    }
}

int player_update(player *p, room rooms[], unsigned short max_x, unsigned short max_y, int *tempo){
    int prev_state = p->anim.state;
    int tile_w = max_x/ROOM_COLS;
    int tile_h = max_y / ROOM_ROWS;

    if (rooms[p->room_id].tiles[(int)(p->y) / tile_h][(int)(p->x) / tile_w] == 11)
        return 1;

    check_damage(p,&rooms[p->room_id]);

    if (p->y > max_y)
        p->is_damaged = 1;

    if((!p->in_action) && (!p->control->down || !p->is_down)){
        if (p->control->left){
            p->direcao=0;
            player_move(p, 1, 0, max_x, max_y);
            if(colision_left(p,&rooms[p->room_id],max_x,max_y))
                p->x = ((int)(p->x - p->side/2) / (tile_w) + 1) * (tile_w) + p->side/2;
        }
        if (p->control->right){
            p->direcao=1;
            player_move(p, 1, 1, max_x, max_y);
            if(colision_right(p,&rooms[p->room_id],max_x,max_y))
                p->x = ((int)(p->x + p->side/2 - 1) / (tile_w)) * (tile_w) - p->side/2;
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
            p->y = ((int)(p->y - p->side/2) / (tile_h) + 1) * (tile_h) + p->side/2;
            p->gravity = 3.0f;
        }

        if(colision_bottom(p,&rooms[p->room_id],max_x,max_y)){
            p->y = ((int)(p->y + p->side/2) / (tile_h)) * (tile_h) - p->side/2;
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
    }

    if (p->is_damaged){
        *tempo = *tempo - 5;
        p->x = 100;
        p->y = 49;
        p->control->down = 0;
        p->is_damaged = 0;
        p->gravity=0;
    }
    
    if (!p->is_down){
        p->anim.state = PULANDO;
        p->anim.velocity = 7;
        if(p->gravity > 0 ){
        if (p->anim.frame >= anim_num_frames(PULANDO) - 3)
            return 0;
        }
    }

    else if (p->control->down){
		p->anim.state= DOWN;
        p->anim.velocity=10;
        if (p->anim.frame >= anim_num_frames(DOWN) - 1){
            p->anim.frame = anim_num_frames(DOWN) - 1;
            return 0;
        }
    }
    else if (p->in_action){
        p->anim.state = ACAO;
        p->anim.velocity = 4;
        if (p->anim.frame >= anim_num_frames(ACAO) -1 ){
            p->in_action=0;
            rooms[p->room_id].tiles[(int)(p->y) / tile_h][(int)(p->x) / tile_w] = TILE_BACK;
            *tempo += 30;
            p->in_action = 0;
            p->anim.frame = 0;
            p->anim.timer = 0;
        }
    }
    
    else if (!p->control->left && !p->control->right){
        p->anim.state = IDLE;
        p->anim.velocity=25;
    }
    else{
        p->anim.state = CORRENDO;
        p->anim.velocity = 5;
    } 


    if (prev_state != p->anim.state){
        p->anim.timer = 0;
        p->anim.frame = 0;
        p->anim.frame_count = anim_num_frames(p->anim.state);
    }   
    anim_update(&p->anim);
    return 0;
}

void player_destroy(player *p){																													//Implementação da função "player_destroy"
	joystick_destroy(p->control);																											//Destrói controle do quadrado (!)
	free(p);																																//Libera a memória do quadrado na heap
}