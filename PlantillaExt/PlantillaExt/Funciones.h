// Tecnologico de Costa Rica - Laboratorio Allegro - Katerine Guzman Flores (2019390523) - Jose Pablo Aguero Mora (2021126372) - Prof: Victor Garro Abarca
// Funciones.h

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


using namespace std;
#pragma warning(disable:4996);
#define FPS 30.0
#pragma once

void juego() {

	if (!al_init()) {
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
		return;
	}

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	al_init_primitives_addon();
	al_install_keyboard();
	int ResX = 800;
	int ResY = 500;

	ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_BITMAP* Personaje = al_load_bitmap("bbear.png");
	ALLEGRO_BITMAP* Npc = al_load_bitmap("coolbug.png");
	ALLEGRO_BITMAP* Fondo = al_load_bitmap("room.jpg");
	ALLEGRO_FONT* Fuente = al_load_font("bladeRunner.TTF", 40, NULL);

	al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
	al_register_event_source(cola_eventos, al_get_keyboard_event_source());

	bool hecho = true;

	int PosJugX = 50; // Posición inicial del personaje
	int PosJugY = 380;

	int Dir = 0;
	int mVert = 0;
	int cont = 1;

	int PosBugX = 200; // Posición inicial del NPC
	int PosBugY = 320;
	int dirBug = 0;

	al_start_timer(timer);

	while (hecho) {

		ALLEGRO_EVENT eventos;
		al_wait_for_event(cola_eventos, &eventos);
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (eventos.keyboard.keycode) {
			case ALLEGRO_KEY_LEFT:
				Dir = 1;
				break;
			case ALLEGRO_KEY_RIGHT:
				Dir = 2;
				break;
			case ALLEGRO_KEY_A:
				Dir = 1;
				break;
			case ALLEGRO_KEY_D:
				Dir = 2;
				break;
			case ALLEGRO_KEY_UP:
				mVert = 1;
				break;
			case ALLEGRO_KEY_DOWN:
				mVert = 2;
				break;
			case ALLEGRO_KEY_W:
				mVert = 1;
				break;
			case ALLEGRO_KEY_S:
				mVert = 2;
				break;
			}
		}
		if (eventos.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (eventos.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				hecho = false;
				break;
			case ALLEGRO_KEY_LEFT:
				Dir = 0;
				break;
			case ALLEGRO_KEY_RIGHT:
				Dir = 0;
				break;
			case ALLEGRO_KEY_A:
				Dir = 0;
				break;
			case ALLEGRO_KEY_D:
				Dir = 0;
				break;
			case ALLEGRO_KEY_UP:
				mVert = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				mVert = 0;
				break;
			case ALLEGRO_KEY_W:
				mVert = 0;
				break;
			case ALLEGRO_KEY_S:
				mVert = 0;
				break;
			}
		}
		if (eventos.type == ALLEGRO_EVENT_TIMER)
		{
			if (eventos.timer.source == timer)
			{
				switch (Dir) {
				case 1:
					if (PosJugX >= 30)
						PosJugX = PosJugX - 5;
					break;
				case 2:
					if (PosJugX <= 770)
						PosJugX = PosJugX + 5;
					break;
				}
				switch (mVert) {
				case 1:
					if (PosJugY >= -15)
						PosJugY = PosJugY - 5;
					break;
				case 2:
					if (PosJugY <= 400)
						PosJugY = PosJugY + 5;
					break;
				}
				if (PosBugX == 380) {
					dirBug = 1;
				}
				if (PosBugX == 200) {
					dirBug = 0;
				}
				if (PosBugX < 381 && dirBug == 0) {
					PosBugX += 3;
				}
				if (PosBugX >= 200 && dirBug == 1) {
					PosBugX -= 3;
				}
				
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_scaled_bitmap(Fondo, 100, 0, 1920, 1080, 0, 0, 800, 500, NULL);
				al_draw_text(Fuente, al_map_rgb(0, 255, 255), 140, 35, ALLEGRO_ALIGN_LEFT, "Practica de Allegro"); //
				al_draw_bitmap(Personaje, PosJugX - 50, PosJugY, NULL); // Cambiado
				al_draw_bitmap(Npc, PosBugX, PosBugY, NULL); // Cambiado
			}
		}
		al_flip_display();
	}
	al_destroy_event_queue(cola_eventos);
	al_destroy_font(Fuente);
	al_destroy_timer(timer);
}

