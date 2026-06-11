
#include <allegro5/allegro5.h>													
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>	
#include <string.h>													
#include "Player.h"
#include "Rooms/Room.h"
#include "Rooms/Rooms.h"
#include <stdio.h>

#define GAME_W 640
#define GAME_H 480
#define FRAME_W 48
#define FRAME_H 48
#define PLAYER_SCALE 1.12
#define TEMPO_VIDA 120

int main(){
	//inicializações da allegro
	al_init();																		
	al_install_keyboard();																		
	al_init_font_addon();
	al_init_primitives_addon();	

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); // deixa a tela fullscreen

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);		//define o fps do jogo
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();	//Cria a fila de eventos
	ALLEGRO_FONT* font = al_create_builtin_font();			//Carrega uma fonte padrão para escrever na tela
	ALLEGRO_DISPLAY* disp = al_create_display(640, 480);	//Cria uma janela padrão para o programa 
	al_init_image_addon(); // precisa disso pra carregar imagens

	//pego o tamanho da sua tela fullscreen
	int real_w = al_get_display_width(disp);
	int real_h = al_get_display_height(disp);
	
	float scale = (float)real_h / GAME_H; //escala do jogo

	float offset_x = (real_w - GAME_W * scale) / 2.0f; // centraliza a tela do jogo para o meio
	float offset_y = 0.0f; //o jogo deve sempre ocupar 100% da vertical
	
	//permite transformar em escala os desenhos da tela
	ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_scale_transform(&transform, scale, scale);
	al_translate_transform(&transform, offset_x, offset_y);


	ALLEGRO_BITMAP* game_over = al_load_bitmap("Assets/game_over.png") ;
	ALLEGRO_BITMAP* game_menu = al_load_bitmap("Assets/menu_inicial.png") ;

	ALLEGRO_BITMAP* tile_sprites[11];
	tile_sprites[TILE_BACK] = al_load_bitmap("Assets/teste3.png");
	tile_sprites[TILE_WALL]  = al_load_bitmap("Assets/parede.png");
	tile_sprites[TILE_FLOOR1] = al_load_bitmap("Assets/chao.png"); 
	tile_sprites[TILE_FLOOR2] = al_load_bitmap("Assets/chao3.png");
	tile_sprites[TILE_SPIKE] = al_load_bitmap("Assets/spike.png"); 
	tile_sprites[TILE_EMPTY] = al_load_bitmap("Assets/back3.png");
	tile_sprites[TILE_LASER] = NULL;
	tile_sprites[TILE_AIR_SPIKE] = al_load_bitmap("Assets/air_spike.png");
	tile_sprites[TILE_MOVING_SPIKE] = al_load_bitmap("Assets/moving_spike.png");
	tile_sprites[TILE_RED_PILL] = al_load_bitmap("Assets/red_pill.png");

	ALLEGRO_BITMAP* anim_sheets[5]; // um por estado
	anim_sheets[IDLE] = al_load_bitmap("Assets/player/player_idle.png");
	anim_sheets[CORRENDO]= al_load_bitmap("Assets/player/player_run.png");	
	anim_sheets[PULANDO]= al_load_bitmap("Assets/player/player_jump.png");	
	anim_sheets[DOWN]= al_load_bitmap("Assets/player/player_down.png");
	anim_sheets[ACAO]= al_load_bitmap("Assets/player/player_action.png");
	// indica que eventos de teclado, tela e tempo vão ativar nossa fila de eventos
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	room rooms[] = {room0,room1,room2};

	player* player = player_create(GAME_W/35, GAME_W/2, GAME_H/2, GAME_W, GAME_H,10);
	if (!player) return 1;	

	float tile_w = GAME_W / ROOM_COLS;
	float tile_h = GAME_H / ROOM_ROWS;

	for (int i = 0; i < 3; i++)
		room_build_obstacles(&rooms[i],tile_w,tile_h);

	ALLEGRO_EVENT event= {0};
	int game_start = 0;
	int death = 0;
	int frames = 0;
    int tempo_restante = TEMPO_VIDA;
    char texto_timer[12]; 
    

	al_start_timer(timer);

	//laço principal do nosso programa															
	while(1){
		al_wait_for_event(queue, &event); //func que observa e coloca eventos na fila (os que falamos que é pra ativar)	
		//eventos de relogio: o que precisa acontecer a cada frame

		if (event.type == 30){
			ALLEGRO_TRANSFORM identity;
			al_identity_transform(&identity);
			al_use_transform(&identity);
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_use_transform(&transform);
			al_draw_filled_rectangle(0, 0, GAME_W, GAME_H, al_map_rgb(1, 1, 1));
			if(!game_start){
				if(!death)
					al_draw_bitmap(game_menu, 0, 0, 0);
				else
					al_draw_bitmap(game_over, 0, 0, 0);
			}
			else{
				frames++;
				if(frames >= 30){
					if(tempo_restante > 0)
						tempo_restante--;
					frames = 0;	
				}
				if (tempo_restante <= 0){
					game_start = 0;	
					death = 1;
					tempo_restante = TEMPO_VIDA;
					player->x = GAME_W/2;
					player->y = GAME_H/2;	
				}
				sprintf(texto_timer, "%d", tempo_restante);

				//calcula a gravidade
				player_update(player,rooms, GAME_W, GAME_H, &tempo_restante);

				room_update(&rooms[player->room_id],player->x, player->y,tile_w, tile_h);
			
				//pinta a tela e os personagens
				room_draw(&rooms[player->room_id],tile_w,tile_h,tile_sprites);	
			
				int flip = (player->direcao == 0) ? ALLEGRO_FLIP_HORIZONTAL : 0;
				ALLEGRO_BITMAP* frame = al_create_sub_bitmap(
				anim_sheets[player->anim.state],
				player->anim.frame * FRAME_W, 0,  // avança 8px por frame
				FRAME_W, FRAME_H
				);

				// desenha escalado e espelhado se necessário
				al_draw_scaled_bitmap(
				frame,
				0, 0,
				FRAME_W, FRAME_H,
				player->x - (player->side * PLAYER_SCALE) / 2,  // centraliza na posição do player
				player->y - (player->side * PLAYER_SCALE) / 2 - 1, // -1 frame pra centralizar de volta o jogador
				player->side * PLAYER_SCALE,
				player->side * PLAYER_SCALE,
				flip
				);
				al_destroy_bitmap(frame); 
				al_draw_text(font, al_map_rgb(255, 255, 255), 550, 50, 0, texto_timer);
			}	
			al_flip_display();
		}
		else if((event.type == 10) || (event.type == 12)){	//eventos de teclado 
			
			if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){ // pulou (espaço)
				joystick_up(player->control);
				if((event.type == 12 && player->gravity < 0))// se o jogador soltar o espaço ele pula menos!
					player->gravity *= 0.5;
			}

			else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
				game_start = 1;
				death = 0;
			}

			else if(event.keyboard.keycode == ALLEGRO_KEY_E){
				player->in_action=1;
				player->anim.frame = -2;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN){
				joystick_down(player->control);
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) //movimento pra esquerda (a ou setinha)
				joystick_left(player->control);
			
			else if(event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) //movimento pra direita (d ou setinha)
				joystick_right(player->control);
				
			else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !game_start) break;	
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
	}

	//funções destrutoras para limpar a casa antes do programa acabar
	for (int i = 0; i < 10; i++)
    if (tile_sprites[i])
        al_destroy_bitmap(tile_sprites[i]);

	for (int i = 0; i < 4; i++)
    if (anim_sheets[i])
        al_destroy_bitmap(anim_sheets[i]);
		
	al_destroy_bitmap(game_menu);
	al_destroy_bitmap(game_over);
	player_destroy(player);
	al_destroy_font(font);															
	al_destroy_display(disp);														
	al_destroy_timer(timer);														
	al_destroy_event_queue(queue);													

	return 0;
}