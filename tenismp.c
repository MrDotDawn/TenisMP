#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#define MAX_BOLAS 12
#define MAXIMUM 32480

const int SCREEN_W = 1080;
const int SCREEN_H = 720;

const int JOGADOR_W = 120;
const int JOGADOR_H = 20;
const float FPS = 60;

typedef struct Jogador {
	float x,y,speed;
	ALLEGRO_COLOR cor;
	int up, down, left, right;
} Jogador;

typedef struct Bola {
	float x;
	float y;
	float raio;
	float dx;
	float dy;
	int id;
	int onscreen;
	ALLEGRO_COLOR cor;
} Bola;

typedef struct OutOfBounds{
	float x, y;
}Oob;

void criaBola(Bola *b, int bolas_criadas) {
			b->y = 0;
			b->x = 0;
			b->cor = al_map_rgb(255,200,0);
			b->raio = 3+rand()%10;
			b->dx = 0;
			b->dy = 0;
			b->id = bolas_criadas;
			printf("Bola criada: id %d\n", b->id);

}

void PosicaoBola(Bola *b, int next_bola){
	switch (next_bola)
	{
		case 0:
			b->x = SCREEN_W;
			b->y = 0;
			b->dx = -2;
			b->dy = 2;
			b->onscreen = 1;
			break;

		case 1:
		b->x = 0;
		b->y = 0;
		b->dx = 2;
		b->dy = 2;
		b->onscreen = 1;
		break;

		case 2:
			b->x = 0;
			b->y = SCREEN_H;
			b->dx = 2;
			b->dy = -2;
			b->onscreen = 1;
			break;

		case 3:
			b->x = SCREEN_W;
			b->y = SCREEN_H;
			b->dx = -2;
			b->dy = -2;
			b->onscreen = 1;
			break;
	}
}

void initOob(Oob *Oob){
	Oob->x = 0;
	Oob->y = -10;
}

void initJogador(Jogador *jogador){
	jogador->x = SCREEN_W/2 - 80;
	jogador->y = SCREEN_H/2 + 40;
	jogador->speed = 7.0;
	jogador->up = 0;
	jogador->down = 0;
	jogador->left = 0;
	jogador->right = 0;
	jogador->cor = al_map_rgb(0, 0, 0);
}

void draw_scenario() {
	al_clear_to_color(al_map_rgb(0,144,0));
	al_draw_filled_rectangle(0, SCREEN_H/2 - 5, SCREEN_W, SCREEN_H/2 + 5,
   al_map_rgb(255,255,0));
}

void draw_jogador(Jogador *jogador) {
	al_draw_filled_rectangle(jogador->x, jogador->y, jogador->x+JOGADOR_W, jogador->y+JOGADOR_H,
   al_map_rgb(0,0,0));
}

void draw_oob(Oob *Oob) {
	al_draw_filled_rectangle(-40, -10, SCREEN_W + 40, -10, al_map_rgb(255,255,0));
}

void jogador2(Jogador *jogador){
	jogador->y =  SCREEN_H/2 -40;
}

void Oob2(Oob *Oob){
	Oob->y = SCREEN_H+10;
}

void add_criada(int *bolas_criadas){
	(*bolas_criadas)++;
	//printf("Bolas criadas: %d\n", *bolas_criadas);
}

void add_next(int *next_bola){
	if ((*next_bola) >= 3){
		(*next_bola) = 0;
	}else{
		(*next_bola)++;
	}
	//printf("Next: %d\n", *next_bola);
}

void atualiza_kontador(int *kontador_prox_respawn){
	if ((*kontador_prox_respawn) >=11){
		(*kontador_prox_respawn) = 0;
	}else{
		(*kontador_prox_respawn)++;
	}
}

void respawn_bola(Bola bolas[], int next_bola, int kontador_prox_respawn){
	//tá funcionando porra
	int contagem = 0;
	for (int i = kontador_prox_respawn; i < MAX_BOLAS; i++){
		if (bolas[i].onscreen == 0 && contagem == 0){
			printf("PRE\nOn screen: %d, id: %d\n", bolas[i].onscreen, bolas[i].id);
			PosicaoBola(&bolas[i], next_bola);
			printf("POS\nOn screen: %d, id: %d\n", bolas[i].onscreen, bolas[i].id);
			contagem++;
		}else{
			continue;
		}
	}
}

int on_off_screen(Bola *b){
	if (b->y > SCREEN_H || b->y < SCREEN_H){
		b->onscreen = 0;
	}else{
		b->onscreen = 1;
	}
}

void desenhaBola(Bola *b) {
	al_draw_filled_circle(b->x, b->y, b->raio, b->cor);
}

void update_jogador(Jogador jogadores[], int i) {
	if (i == 0){
		if(jogadores[0].up && jogadores[0].y > SCREEN_H/2 + JOGADOR_H) {
			jogadores[0].y -= jogadores[0].speed;
		}
		if (jogadores[0].down && jogadores[0].y < SCREEN_H - JOGADOR_H){
			jogadores[0].y += jogadores[0].speed;
		}

		if (jogadores[0].left && jogadores[0].x > 0){
				jogadores[0].x -= jogadores[0].speed;
		}	
		if (jogadores[0].right && jogadores[0].x < SCREEN_W - JOGADOR_W){
				jogadores[0].x += jogadores[0].speed;
		}
	}else{

		if(jogadores[1].up && jogadores[1].y > 0) {
			jogadores[1].y -= jogadores[1].speed;
		}
		if (jogadores[1].down && jogadores[1].y < SCREEN_H/2 - JOGADOR_H*2){
			jogadores[1].y += jogadores[1].speed;
		}
		if (jogadores[1].left && jogadores[1].x > 0){
			jogadores[1].x -= jogadores[1].speed;
		}	
		if (jogadores[1].right && jogadores[1].x < SCREEN_W - JOGADOR_W){
			jogadores[1].x += jogadores[1].speed;
		}
	}
}

void colisaobolaparede(Bola *bola){
	if(bola->x > SCREEN_W || bola->x < 0){
		bola->dx = -bola->dx;
	}
}

void update_bola(Bola *bola){
	bola->x += bola->dx;
	bola->y += bola->dy;
}

float flip_xbola(int oldx) {
	int s=1;
	if(oldx < 0) s=-1;
	oldx = oldx*(0.25+2*(float)rand()/RAND_MAX);
	if(s*oldx < 2)
		oldx = 2*s;
	else if(s*oldx > 2 && s*oldx < 10)
		oldx = 8*s;
	else if(s*oldx > 10)
		oldx = 10*s;
	return oldx;
}

float flip_ybola(int oldy) {
	int s=1;
	if(oldy < 0) s=-1;
	oldy = oldy*(0.25+1.1*(float)rand()/RAND_MAX);
	if(s*oldy < 2)
		oldy = 2*s;
	else if(s*oldy > 2 && s*oldy < 10)
		oldy = 8*s;
	else if(s*oldy > 10)
		oldy = 10*s;
	return -oldy;
}

void ColisaoOob(Bola *bola, Oob Oobs[], int j, int *pontos_p1, int *pontos_p2){
	int contador = 0;
	if (j==0){
		if(bola->x <= Oobs[0].x + SCREEN_W && bola->x >= Oobs[0].x && bola->y >= Oobs[0].y-10 && bola->y <= Oobs[0].y && contador == 0){
			contador++;
			(*pontos_p1)++;
		}
	}else if(j==1){
		if (bola->x <= Oobs[1].x + SCREEN_W && bola->x >= Oobs[1].x && bola->y <= Oobs[1].y+10 && bola->y >= Oobs[1].y && contador == 0){
			contador++;
			(*pontos_p2)++;
		}
	}

}

void colisaojogadorbola(Bola *bola, Jogador jogadores[], int j, ALLEGRO_EVENT ev){
	switch (ev.keyboard.keycode){
		case ALLEGRO_KEY_SPACE:
			if (j==0){
					if(bola->x <= jogadores[0].x+JOGADOR_W && bola->x >= jogadores[0].x && bola->y <= jogadores[0].y+JOGADOR_H && bola->y + bola->raio >= jogadores[0].y){
						float oldx = bola->dx, oldy = bola->dy;
						bola->dx = 0;
						bola->dy = 0;
						bola->dx = flip_xbola(oldx);
						bola->dy = flip_ybola(oldy);
						if (bola->dy > 0){
							bola->dy *= -1;
						}
					}
				}
		break;
	
		case ALLEGRO_KEY_ALT:
			if (j==1){
					if(bola->x <= jogadores[1].x+JOGADOR_W && bola->x >= jogadores[1].x && bola->y <= jogadores[1].y+JOGADOR_H && bola->y + bola->raio >= jogadores[1].y){
						float oldx = bola->dx, oldy = bola->dy;
						bola->dx = 0;
						bola->dy = 0;
						bola->dx = flip_xbola(oldx);
						bola->dy = flip_ybola(oldy);
						if (bola->dy < 0){
							bola->dy *= -1;
						}
					}
				}
		break;
	}
}

void movimenta_jogador(Jogador jogadores[], int j, ALLEGRO_EVENT ev){
	switch(ev.keyboard.keycode) {
			if (j == 0 ){
				case (ALLEGRO_KEY_LEFT):
					if (jogadores[0].x > 0){
						jogadores[0].left = 1;
					}
				break;

				case (ALLEGRO_KEY_RIGHT):
					if (jogadores[0].x < SCREEN_W){
						jogadores[0].right = 1;
					}
				break;

				case (ALLEGRO_KEY_UP):
					if (jogadores[0].y > 0){
						jogadores[0].up = 1;
					}
				break;

				case (ALLEGRO_KEY_DOWN):
					if (jogadores[0].y < SCREEN_H){
						jogadores[0].down = 1;
					}
				break;

			}else{
				case (ALLEGRO_KEY_A):
					if (jogadores[1].x > 0){
						jogadores[1].left = 1;
					}
				break;

				case (ALLEGRO_KEY_D):
					if (jogadores[1].x < SCREEN_W){
						jogadores[1].right = 1;
					}
				break;

				case (ALLEGRO_KEY_W):
					if (jogadores[1].y > 0){
						jogadores[1].up = 1;
					}
				break;

				case (ALLEGRO_KEY_S):
					if (jogadores[1].y < SCREEN_H/2){
						jogadores[1].down = 1;
					}
				break;
			}
			}
}

void release_jogador(Jogador jogadores[], int i, ALLEGRO_EVENT ev){

	switch(ev.keyboard.keycode) {
				if (i == 0){
				case (ALLEGRO_KEY_LEFT):
					jogadores[0].left = 0;
				break;

				case (ALLEGRO_KEY_RIGHT):
					jogadores[0].right = 0;
				break;

				case (ALLEGRO_KEY_UP):
					jogadores[0].up = 0;
				break;

				case (ALLEGRO_KEY_DOWN):
					jogadores[0].down = 0;
				break;

			}else{
				case (ALLEGRO_KEY_A):
					jogadores[1].left = 0;
				break;

				case (ALLEGRO_KEY_D):
					jogadores[1].right = 0;
				break;

				case (ALLEGRO_KEY_W):
					jogadores[1].up = 0;
				break;

				case (ALLEGRO_KEY_S):
					jogadores[1].down = 0;
				break;
			}
			}
}

void teleporta_jogador(Jogador *jogador2, ALLEGRO_EVENT ev){
	switch (ev.keyboard.keycode){
		case ALLEGRO_KEY_R:
			jogador2->x = rand()%SCREEN_W;
			jogador2->y = rand()%SCREEN_H - JOGADOR_H;

			if (jogador2->x + JOGADOR_W <= 0){
				jogador2->x += JOGADOR_W;
			}else if(jogador2->x + JOGADOR_W >= SCREEN_H){
				jogador2->x -= JOGADOR_W;
			}
			if (jogador2->y <= SCREEN_H/2 + JOGADOR_H){
				float distancia = SCREEN_H/2 - jogador2->y + JOGADOR_H;
				jogador2->y += distancia;
			}
	}

}

int main(int argc, char **argv){

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	

	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//instala o mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *font = al_load_font("arial.ttf", 32, 1);   
	if(font == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   


	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());  

	int bolas_criadas = 0;
	int next_bola = 0;
	int pontos_p1 = 0;
	int pontos_p2 = 0;
	int kontador_prox_respawn = 0;
	int segundos = 0;
	char placar1[10];
	char placar2[10];
	int x = 0;
	int y = 0;

	Jogador jogadores[2];
	for (int i = 0; i<2; i++){
		initJogador(&jogadores[i]);
	}

	jogador2(&jogadores[1]);

	Bola bolas[MAX_BOLAS];

	Oob Oobs[2];
	for (int i = 0; i <2; i++){
		initOob(&Oobs[i]);
	}

	Oob2(&Oobs[1]);
	
	int playing = 1;
	//inicia o temporizador
	al_start_timer(timer);
	while(playing){
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for temporizador)
		if(ev.type == ALLEGRO_EVENT_TIMER) {

			x = pontos_p1/6;
			y = pontos_p2/6;

			sprintf(placar1, "%d", x);
			sprintf(placar2, "%d", y);

			draw_scenario();

			for (int i = 0; i < MAX_BOLAS; i++){
				for (int j = 0; j< 2; j++){
					ColisaoOob(&bolas[i], Oobs, j, &pontos_p1, &pontos_p2);
				}
			}

			al_draw_text(font, al_map_rgb(255,255,255), 0, 0, ALLEGRO_ALIGN_LEFT, &placar1[0]);
			al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W, 0, ALLEGRO_ALIGN_RIGHT, &placar2[0]);
			
			for (int i = 0; i<2; i++){
				update_jogador(jogadores, i);
			}

			for (int i = 0; i<2; i++){
				draw_jogador(&jogadores[i]);
			}

			for (int i = 0; i <MAX_BOLAS; i++){
				colisaobolaparede(&bolas[i]);
				on_off_screen(&bolas[i]);
			}

			
			if(al_get_timer_count(timer)%(int)FPS == 0){
				if (segundos == 4){
					segundos = 0;
					if (bolas_criadas < MAX_BOLAS){
						criaBola(&bolas[bolas_criadas], bolas_criadas);
						PosicaoBola(&bolas[bolas_criadas], next_bola);
						add_criada(&bolas_criadas);
						add_next(&next_bola);
					}else{
						respawn_bola(bolas,next_bola,kontador_prox_respawn);
						atualiza_kontador(&kontador_prox_respawn);
						add_next(&next_bola);
					}
				}else{
					segundos++;
				}
			}

			for (int i = 0; i <MAX_BOLAS; i++){
				update_bola(&bolas[i]);
				desenhaBola(&bolas[i]);
			}


			if (x >= 10 || y >= 10){
				char final[20];	
		
				al_clear_to_color(al_map_rgb(0,0,0));
				if (x >= 10){
					
					sprintf(final, "Jogador vencedor: 1    Total de pontos: %d", x);	
					al_draw_text(font, al_map_rgb(15, 200, 30), SCREEN_W/4, SCREEN_H/2, 0, final);
				}	

				if (y >= 10){
					sprintf(final, "Jogador vencedor: 2    Total de pontos: %d", y);	
					al_draw_text(font, al_map_rgb(15, 200, 30), SCREEN_W/4, SCREEN_H/2+100, 0, final);
				}
				
				//reinicializa a tela
				al_flip_display();	
				al_rest(3);	
				break;
			}
			
		
			//atualiza a tela
			al_flip_display();

		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}

		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			for (int i = 0; i < MAX_BOLAS; i++){
				for (int j = 0; j< 2; j++){
					movimenta_jogador(jogadores, j, ev);
					colisaojogadorbola(&bolas[i], jogadores, j, ev);
				}
			}
			teleporta_jogador(&jogadores[0], ev);
		}

		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			for (int i = 0; i < 2; i++){
				release_jogador(jogadores, i, ev);
			}
		}

	} //fim do while
     
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	
 
	al_destroy_timer(timer);
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}