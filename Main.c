
#include <allegro5/allegro5.h>													
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>														
#include "Player.h"
#include "Rooms/Room.h"
#include "Rooms/Room1.h"
#include <stdio.h>

int main(){
	//inicializações da allegro
	al_init();																		
	al_install_keyboard();																		
	al_init_primitives_addon();	

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); // deixa a tela fullscreen

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);		//define o fps do jogo
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();	//Cria a fila de eventos
	ALLEGRO_FONT* font = al_create_builtin_font();			//Carrega uma fonte padrão para escrever na tela
	ALLEGRO_DISPLAY* disp = al_create_display(1920, 1080);	//Cria uma janela padrão para o programa 

	//pego o tamanho da sua tela fullscreen
	ALLEGRO_DISPLAY_MODE disp_data;
	al_get_display_mode(0, &disp_data);

	int x_screen = disp_data.width;  // x da tela
	int y_screen = disp_data.height; // y da tela

	// indica que eventos de teclado, tela e tempo vão ativar nossa fila de eventos
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	room rooms[] = {room1,room2,room3};

	player* player = player_create(x_screen/30, x_screen/2, y_screen/2, x_screen, y_screen,10);
	if (!player) return 1;	

	int tile_w = x_screen / ROOM_COLS;
	int tile_h = y_screen / ROOM_ROWS;

	ALLEGRO_EVENT event;
	al_start_timer(timer);

	//laço principal do nosso programa															
	while(1){
		al_wait_for_event(queue, &event); //func que observa e coloca eventos na fila (os que falamos que é pra ativar)	
		
		//eventos de relogio: o que precisa acontecer a cada frame
		if (event.type == 30){
			//calcula a gravidade
			player_update(player,rooms, x_screen, y_screen);
			//pinta a tela e os personagens
			al_clear_to_color(al_map_rgb(0, 0, 0));	
    		room_draw(&player->room,tile_w,tile_h);
			al_draw_filled_rectangle(player->x-player->side/2, player->y-player->side/2, player->x+player->side/2, player->y+player->side/2, al_map_rgb(255, 0, 0)); 
			al_flip_display();
		}
		else if((event.type == 10) || (event.type == 12)){	//eventos de teclado 
			
			if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){ // pulou (espaço)
				joystick_up(player->control);
				if((event.type == 12 && player->gravity < 0))// se o jogador soltar o espaço ele pula menos!
					player->gravity *= 0.5;
			}

			else if(event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT) //movimento pra esquerda (a ou setinha)
				joystick_left(player->control);
			
			else if(event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT) //movimento pra direita (d ou setinha)
				joystick_right(player->control);
			
			
			
		}
		else if (event.type == 42) break;	//Evento de clique no "X" de fechamento da tela
	
	}

	//funções destrutoras para limpar a casa antes do programa acabar
	player_destroy(player);
	al_destroy_font(font);															
	al_destroy_display(disp);														
	al_destroy_timer(timer);														
	al_destroy_event_queue(queue);													

	return 0;
}