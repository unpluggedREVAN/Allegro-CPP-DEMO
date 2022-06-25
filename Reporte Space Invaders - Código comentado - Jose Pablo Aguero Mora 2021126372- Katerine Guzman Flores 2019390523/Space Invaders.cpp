// Tecnológico de Costa Rica - Estructuras de Datos - Grupo 02- Reporte Space Invaders - Jose Pablo Aguero Mora 2021126372- Katerine Guzman Flores 2019390523


// este archivo es como un main que va a tener su propia cola de eventos pero que tiene el menu y adentro de este se inicializa el juego 
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Juego.h"

using namespace std;
#pragma warning(disable:4996); // habilita advertencias que visual con algunas funciones de guardar archivos en memoria secundaria
#define FPS 60.0

// Allegro no tiene animacion como tal pero mediante ciertas configuraciones se puede lograr, para eso son esas variables globales
const int maxFrame = 1;
int frameactual = 0;
int contadorDeFrames = 0;
int Delay = 25;
int alturaDeFrame = 1080;
int AnchoDeFrame = 1920;
//Se definen variables que serán usadas para la animación del menú
int main() {
	//Si no se inicializa Allegro se lanza el error
	if (!al_init()) {
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
		return -1;
	}
	//Se instalan los complementos y periféricos
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(7); // reservar pistas de audio
	al_install_mouse();

	ALLEGRO_MONITOR_INFO monitor; // con esto el juego se adapta a todas las resoluciones de pantallas
	al_get_monitor_info(0, &monitor);//Se obtiene la información del monitor como su resolución mediante diferencia de puntos finales e iniciales
	const int RX = monitor.x2 - monitor.x1; // x del monitor
	const int RY = monitor.y2 - monitor.y1; // y del monitor

	// se hace el display
	al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);//Se modifica el display para que sea pantalla completa

	ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);

	al_set_window_title(pantalla, "Space Invaders");//Se le pone un título a la ventana
	if (!pantalla)
	{
		//Si el display no se ejecuta se lanza el mensaje de error
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	//Se crean todos los punteros propios de Allegro y se carga el multimedia y los timers
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER* timerD = al_create_timer(1.0 / 15);

	ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();

	ALLEGRO_FONT* fuente1;
	ALLEGRO_FONT* fuente2;
	ALLEGRO_FONT* fuente3;

	ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/Espacio clone.png"); // fondo que se pueda ver
	ALLEGRO_BITMAP* Flecha = al_load_bitmap("Imagenes/Flecha.png"); // flecha para pasar los resultados
	ALLEGRO_BITMAP* Instrucc = al_load_bitmap("Imagenes/Instrucc.png"); // bitmaps para tecla de espacio y flechas

	ALLEGRO_SAMPLE* musica = al_load_sample("Musica/Musica2.wav");

	// se cargan cada una de las fuentes para escribir en el menu
	fuente1 = al_load_font("pixel.ttf", 80, NULL);
	fuente2 = al_load_font("pixel.ttf", 50, NULL);
	fuente3 = al_load_font("pixel.ttf", 30, NULL);

	//Se registran todas las fuentes de eventos(timers y periféricos) en la cola de eventos

	al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
	al_register_event_source(cola_eventos, al_get_timer_event_source(timerD));
	al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
	al_register_event_source(cola_eventos, al_get_keyboard_event_source());
	al_register_event_source(cola_eventos, al_get_mouse_event_source());

	// variables que se necesitan
	bool hecho = true; // mantiene el ciclo infinito del while del juego
	int mousex = 0;
	int mousey = 0;
	int a = 0;
	int inicio = 0;
	bool modo;

	// intento de hacer diferentes dimensiones del juego pero no es necesario porque arriba ya esta lo del ajuste de pantalla
	//Se obtienen las dimensiones de la pantalla en donde se proyecta el juego para que este se ajuste a las dimensiones
	int X = al_get_display_width(pantalla);
	int Y = al_get_display_height(pantalla);

	//Se reproduce la música del menú principal e inician timers
	al_play_sample(musica, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
	al_start_timer(timer);
	al_start_timer(timerD);

	// COLA DE EVENTOS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	while (hecho)//Se empieza la cola de eventos indefinidamente
	{
		ALLEGRO_EVENT eventos; // se crea un nuevo evento
		al_wait_for_event(cola_eventos, &eventos); // espera por que ocurra algun cambio en la cola de eventos (teclado, mouse, timer)

		if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES) // si ocurre un evento en la posicion del mouse
		{
			//Se registra la posición en x y y del mouse
			mousex = eventos.mouse.x;
			mousey = eventos.mouse.y;

		}
		if (eventos.type == ALLEGRO_EVENT_TIMER) // si es evento del timer
		{
			if (eventos.timer.source == timer)
			{  // parpadeo 
				if (contadorDeFrames++ >= Delay) {//Se declara un contador de frames que logra avanzar la animación del menú principal
					if (frameactual++ >= maxFrame) {
						frameactual = 0;
					}
					contadorDeFrames = 0;
				}
				//Se dibuja el fondo animado y las opciones del menú en el display
				al_clear_to_color(al_map_rgb(0, 0, 0)); // se limpia pantalla a negro y va a estar escalado ya que la imagen puede variar dependeiendo de la resolucion
				al_draw_scaled_bitmap(Fondo, 0, frameactual * alturaDeFrame, AnchoDeFrame, alturaDeFrame, 0, 0, RX, RY, 0);
				al_draw_text(fuente1, al_map_rgb(250, 250, 15), X / 2, (RY * (150.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SPACE INVADERS");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "JUGAR"); // fuente, color, posicion(mitad de pantalla en x y en y va a cambiar dependiendo de resolution, por eso hay un factor que multiplica)
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "DEMO");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (390.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (460.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "INSTRUCCIONES");
				al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
			}
		}
		//Si se posiciona el mouse en las coordenadas donde indica la opción jugar
		if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * 260.0 / 768.0) && mousey <= (RY * 290.0 / 768.0))) {
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "JUGAR"); // Se muestra en color amarillo
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					//Si se presiona el click del mouse se inicia el juego y se indican las vidas totales y el nivel, si el juego termina se avanza de nivel hasta que se pierdan las vidas y el modo determina si es maquina vs maquina o jugador vs maquina
					al_destroy_sample(musica);
					al_destroy_display(pantalla);
					int vida = 3;
					int nivel = 1;
					modo = true;
					while (vida != 0) {
						vida = juego(nivel, vida, modo); // se inicia la funcion juego
						nivel = nivel + 1;
					}
					main();//Se vuelve a lanzar el menú si se sale del juego
					hecho = false;
				}
			}
		}
		//Si el mouse se posiciona sobre DEMO se pinta de amarillo y si se hace click se lanza el juego pero en modo DEMO que significa máquina vs  máquina
		if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * 330.0 / 768.0) && mousey <= (RY * 360.0 / 768.0))) {
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (320.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "DEMO");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					al_destroy_sample(musica);
					al_destroy_display(pantalla);
					int vida = 3;
					int nivel = 1;
					modo = false;
					while (vida != 0) {
						vida = juego(nivel, vida, modo);
						nivel = nivel + 1;
					}
					main();
					hecho = false;
				}
			}
		}
		//Si se posiciona el mouse sobre resultados este se tinta de amarillo y si se hace click lanza el menu de resultados donde se pueden ver todas las partidas anteriores
		if ((mousex >= X / 2 - 120 && mousex <= X / 2 + 120) && (mousey >= (RY * (400.0 / 768.0)) && mousey <= (RY * (430.0 / 768.0)))) {
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, RY * (390.0 / 768.0), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					bool salir = false;
					while (!salir) {
						al_wait_for_event(cola_eventos, &eventos);
						if (eventos.type == ALLEGRO_EVENT_TIMER) {
							if (eventos.timer.source == timer) {
								if (contadorDeFrames++ >= Delay) {
									if (frameactual++ >= maxFrame) {
										frameactual = 0;
									}
									contadorDeFrames = 0;
								}
								//Se dibuja el fondo animado otra vez y se lanzan los resultado y felchas para navegar los resultados así como la tecla atrás para volver al menú principal
								al_clear_to_color(al_map_rgb(0, 0, 0));
								al_draw_scaled_bitmap(Fondo, 0, frameactual * alturaDeFrame, AnchoDeFrame, alturaDeFrame, 0, 0, RX, RY, 0);
								al_draw_text(fuente1, al_map_rgb(250, 250, 15), X / 2, (RY * (150.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SPACE INVADERS");
								al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");
								al_draw_bitmap(Flecha, X / 2 - 200, (RY * (300.0 / 768.0)), NULL);
								al_draw_bitmap(Flecha, X / 2 - 200, (RY * (500 / 768.0)), ALLEGRO_FLIP_VERTICAL);
								CargarArchivo(X, Y, fuente3, inicio);
								al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2 - 300, (RY * (510.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");
							}
						}
						if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
						{
							mousex = eventos.mouse.x;
							mousey = eventos.mouse.y;
						}
						//Si el mouse se posiciona en las flechas se pinta de amarillo la flecha y si se presiona se vanza arriba o abajo en los resultados
						if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 155) && (mousey >= 300 && mousey <= 330)) {
							al_draw_tinted_bitmap(Flecha, al_map_rgb(250, 250, 25), X / 2 - 200, (RY * (300.0 / 768.0)), NULL);
							if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
							{
								if (eventos.mouse.button & 1) {
									if (inicio >= 1) {
										inicio = inicio - 1;
									}
								}
							}
						}
						if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 155) && (mousey >= 510 && mousey <= 540)) {
							al_draw_tinted_bitmap(Flecha, al_map_rgb(250, 250, 25), X / 2 - 200, (RY * (500.0 / 768.0)), ALLEGRO_FLIP_VERTICAL);
							if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
							{
								if (eventos.mouse.button & 1) {
									inicio = inicio + 1;
								}
							}
						}
						if ((mousex >= X / 2 - 42 - 300 && mousex <= X / 2 + 42 - 300) && (mousey >= (RY * (520.0 / 768.0)) && mousey <= (RY * (550.0 / 768.0)))) {
							al_draw_text(fuente2, al_map_rgb(250, 250, 15), X / 2 - 300, (RY * (510.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");
							if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
							{
								if (eventos.mouse.button & 1) {
									salir = true;
								}
							}
						}
						al_flip_display();
					}
				}
			}
		}
		//Si se presionan las instrucciones se pueden ver dos bitmaps que indican como jugar el juegp
		if ((mousex >= X / 2 - 120 && mousex <= X / 2 + 120) && (mousey >= (RY * (470.0 / 768.0)) && mousey <= (RY * (500.0 / 768.0)))) { //compara el cuadro que se encurntra sobre el texto y lo pone amarillo
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (460.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "INSTRUCCIONES");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) // si toca mouse
			{
				if (eventos.mouse.button & 1) { // si es boton izquierdo
					bool salir = false;
					while (!salir) { // mientras siga el ciclo tal como en el menu principal
						al_wait_for_event(cola_eventos, &eventos);
						if (eventos.type == ALLEGRO_EVENT_TIMER) {
							if (eventos.timer.source == timer) {
								if (contadorDeFrames++ >= Delay) {//Se vuelve a lanzar la animación
									if (frameactual++ >= maxFrame) {
										frameactual = 0;
									}
									contadorDeFrames = 0;
								}
								//Se dibujan todos los bitmaps, las instrucciones y títulos
								al_clear_to_color(al_map_rgb(0, 0, 0));
								al_draw_scaled_bitmap(Fondo, 0, frameactual * alturaDeFrame, AnchoDeFrame, alturaDeFrame, 0, 0, RX, RY, 0);
								al_draw_text(fuente1, al_map_rgb(250, 250, 15), X / 2, (RY * (150.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SPACE INVADERS");
								al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (250.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "INSTRUCCIONES");
								al_draw_bitmap(Instrucc, X / 2 - 200, (RY * (350.0 / 768.0)), NULL);
								al_draw_text(fuente2, al_map_rgb(250, 250, 250), X / 2, (RY * (510.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");
							}
						}
						if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
						{
							mousex = eventos.mouse.x;
							mousey = eventos.mouse.y;
						}
						if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * (520.0 / 768.0)) && mousey <= (RY * (550.0 / 768.0)))) {
							//Si se presiona atras sale al menú principal
							al_draw_text(fuente2, al_map_rgb(250, 250, 15), X / 2, 510, ALLEGRO_ALIGN_CENTRE, "Atras");
							if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
							{
								if (eventos.mouse.button & 1) {
									salir = true; // logra que se rompa el ciclo
								}
							}
						}
						al_flip_display(); // por cada ciclo que se haga hay que poner al flip display para que se muestren los cambios que se han realizado
					}
				}
			}
		}
		if ((mousex >= X / 2 - 120 && mousex <= X / 2 + 120) && (mousey >= (RY * (540.0 / 768.0)) && mousey <= (RY * (570.0 / 768.0)))) {
			//Si se presiona la  opción de salir se sale de la cola de eventos y termina la aplicación
			al_draw_text(fuente2, al_map_rgb(250, 250, 20), X / 2, (RY * (530.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
			if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (eventos.mouse.button & 1) {
					hecho = false;
				}
			}
		}
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			//Si se presiona la tecla escape se sale del juego
			switch (eventos.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				hecho = false;
			}
		}
		else if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			hecho = false;
		}
		//juego();
		al_flip_display();
	}
}