// Tecnologico de Costa Rica - Laboratorio Allegro - Katerine Guzman Flores (2019390523) - Jose Pablo Aguero Mora (2021126372) - Prof: Victor Garro Abarca
// Main.cpp

#include "Funciones.h"

int main()
{
	if (!al_init())
	{
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar Allegro", NULL, NULL);
		return -1;
	}

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();

	int ResX = 800;
	int ResY = 500;
	int mousex = 0;
	int mousey = 0;

	ALLEGRO_MONITOR_INFO monitor;
	al_get_monitor_info(0, &monitor);
	const int RX = monitor.x2 - monitor.x1;
	const int RY = monitor.y2 - monitor.y1;

	ALLEGRO_DISPLAY* pantalla = al_create_display(ResX, ResY);
	al_set_window_position(pantalla, RX / 4 - ResX / 4, RY / 4 - ResY / 4);
	al_set_window_title(pantalla, "Práctica de Allegro");

	if (!pantalla)
	{
		al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	juego();
	al_destroy_display(pantalla);
}