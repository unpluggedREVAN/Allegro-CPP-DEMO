// Tecnol�gico de Costa Rica - Estructuras de Datos - Grupo 02- Reporte Space Invaders - Jose Pablo Aguero Mora 2021126372- Katerine Guzman Flores 2019390523



#pragma once
// Funciones: Todas las funciones necesarias para que el juego funcione
// Librer�as C+
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <algorithm> 

// Librer�as Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Juego.h"
//Primariamente, se incluyen todas las librer�as necesarias para la realizaci�n del juego
using namespace std;
#pragma warning(disable:4996);//Se desactiva alarma en el manejo de archivos
//Se definen las constantes que definen la pantalla jugable del juego
const int ResX = 700;
const int ResY = 800;
const int diametro = 32;
//Se define la estructura nave que controlar� tanto las naves enemigas como la aliada, esta tendr� un codigo(para enemigos), posiciones en x y y, estado(vivo o muerto) y un puntero a nave para crear la lista enlazada(enemigos)
typedef struct nave {
	int codigo; // identificar cada nave
	int x; // posicion en x y en y
	int y;
	int velocidad; 
	bool estado; // viva o muerta
	nave* Siguiente; // lista enalzada con naves
}*PtrNave;
//Se define la estructura escudo esta ser� utilizada para crear un array de estructuras y as� tener los cuatro  escudos en pantalla
typedef struct escudo {
	//Los objetos codigo se utilizan como multiplicadores en el bitmap region que permite los escudos segmentados
	//Se tienen los  objetos de posici�n(X,Y)  el ancho de cada region de bitmap las vidas y el estado de los escudos
	int codigo;
	int codigo2;
	int X;
	int Y;
	int ancho;
	int vidas;
	bool estado;
};
//Se define la estructura bala que se utilizar� para las balas enemigas y aliadas estas tienen objetos de posici�n(x,y), velocidad, estado y un puntero a bala que servir� para crear la lista enlazada
typedef struct bala {
	int y;
	int x;
	int velocidad;
	bool estado; // si le pego a un enemigo
	bala* Siguiente;
}*PtrBala; // lista enlazada de bala

// Ingresa todos los valores para la estructura y se establecen las osiciones del jugador en pantalla
void inicializarJugador(nave& jugador, int x) {
	//Se crea la funci�n inicializar jugador que ingresar� os valores para cada estructura insertada, en este caso la nave jugador que se inicia a la mitad de la pantalla y en una posici�n fija en Y 
	jugador.x = x / 2;
	jugador.y = ResY - 100;
	jugador.codigo = 3; // vidas del jugador
	jugador.estado = true; // esta vivo
	jugador.Siguiente = NULL; // lista enlazada
}

// Deja la estructura disponible para que pueda incluirse en la lista enlazada
void crearEnemigo(PtrNave& enemigo, int i, int x) {
	//Se crea la funci�n crearEnemigo, esta funci�n se encarga de incializar cada enemigo, lo coloca en una posici�n en la pantalla, activa el enemigo y deja la estructura disponible para unirse a la lista enlazada(Siguiente=NULL) 
	enemigo->codigo = i; // numero que va a estar en la posicion de los enemigos, porque hay muchos enemigos
	enemigo->x = x / 2 - ResX / 2 + (i % 11) * ((ResX - 200) / 10) + 100; // se define posicion del enemigo segun resolution del display
	enemigo->y = ((i / 11) + 1) * diametro / 2 * 4; // se toma en cuenta el codigo donde esta y se va definiendo a donde va a quedar cada uno (11 en cada linea) entonces se dividen en un espacio exacto
	enemigo->velocidad = 1;
	enemigo->estado = true; // ya van a estar vivos
	enemigo->Siguiente = NULL; // deja estructura lista para lista enlazada
}

// Se incluye el enemigo creado
void a�adirEnemigo(PtrNave& enemigos, PtrNave& nuevo) {
	//Se crea la lista enlazada enemigos  mediante el entrelazamiento de las estructuras por medio de sus punteros
	if (enemigos != NULL) {
		nuevo->Siguiente = enemigos;
	}
	enemigos = nuevo;
}

void inicializarEnemigos(PtrNave& enemigos, int x) {
	//Esta funci�n solo se encarga de crear un ciclo en el que se cree un nuevo enemigo y este sea agregado a la lista enlazada
	for (int i = 0; i < 55; i++) {
		PtrNave enemigo = new(nave); // se declara variabel anonima de tipo enemigo
		crearEnemigo(enemigo, i, x);
		a�adirEnemigo(enemigos, enemigo);
		enemigo = NULL;
	}
}
//x / 2 -

int contarBorde(PtrNave& enemigos, int x) {
	//Se asigna ultimo como el inicio de la pantalla del juego
	int ultimo = x / 2 - ResX / 2;  // posicion de la nave entre dos, menos posicion de resolution pantalla entre dos
	PtrNave	Aux = NULL;
	Aux = enemigos;
	//Si la velocidad de las naves es negativa (se desplaza hacia la izquierda) se cambia ultimo por el final de la pantalla de juego (la parte derecha)
	if (Aux->velocidad < 0) {
		ultimo = x / 2 + ResX / 2;
	}
	while (Aux != NULL) {
		if (Aux->estado == true) {
			//Para las naves activas, si la velocidad es negativa se asigna como ultimo la posicion de la nave mas
			//proxima a colisionar con la pared segun la direccion que lleve
			if (Aux->velocidad > 0) {
				if (Aux->x > ultimo) {
					ultimo = Aux->x;
				}
			}
			else {
				if (Aux->x < ultimo) {
					ultimo = Aux->x;
				}
			}
		}
		Aux = Aux->Siguiente;
	}
	return ultimo;
}

int contarEnemigos(PtrNave& enemigos) {
	//Se crea la funci�n contar enemigos esta crea un contador que avanza por la lista enlazada y cada nuevo enlacehace  que el contador avance en +1
	int total = 0;
	PtrNave	Aux = NULL;
	Aux = enemigos;
	while (Aux != NULL) {
		if (Aux->estado == true) {
			total++;//contador +1
		}
		Aux = Aux->Siguiente;//avance en la lista
	}
	return total;//regresa el contador
}

void moverEnemigos(PtrNave& enemigos, int total, int x) {
	//Esta funci�n se encarga de mover los enemigos dependiendo de cuantos queden restantes
	PtrNave	Aux = NULL;
	Aux = enemigos;
	int ultimo = contarBorde(enemigos, x);//Se define el borde 
	bool vuelta = false;
	if (Aux->velocidad < 0) {
		//Cada  enemigo da vuelta hasta llegar a su borde
		if (ultimo + Aux->velocidad - diametro / 2 < x / 2 - ResX / 2) {
			vuelta = true;
		}
	}
	else {
		if (ultimo + Aux->velocidad + diametro / 2 > x / 2 + ResX / 2) {
			vuelta = true;
		}
	}
	while (Aux != NULL) {
		if (vuelta == true) {
			Aux->velocidad = -Aux->velocidad;//Se  define la vuelta como un cambio en el signo de la velocidad
			Aux->y = Aux->y + diametro / 2 * 4;
		}
		Aux->x = Aux->x + Aux->velocidad;
		Aux = Aux->Siguiente;
	}
}

int colision(nave& enemigos, bala& Bala) {
	//Se define la funci�n colisi�n la cual establece un modo de colisi�n estilo hitbox en el cual si los cuadrados se ven traslapados por el hitbox se detecta como colision
	PtrNave	Aux = NULL;
	Aux = &enemigos;
	while (Aux != NULL) { // recorre la lista de enemigos 
		if (Aux->estado == true) { // de los enemigos que esten vivos compara si el cuadro de la bala llega a intersercan con el cuadro de enemigo
			if ((Aux->x + diametro / 2 > Bala.x && Aux->x - diametro / 2 < Bala.x) && (Aux->y + diametro / 2 >= Bala.y - 25 && Bala.y > Aux->y - diametro / 2)) {//Se forma rectangulo de hitbox
				//Si se detecta una colision se elimina el enemigo, se elimina la bala de pantalla y se retorna la nave eliminada
				Aux->estado = false;
				Bala.y = ResY;
				Bala.estado = false;
				return Aux->codigo; // retorna el cosigo del enemigo al que impacto
			}
		}
		Aux = Aux->Siguiente;
	}
	return 56;
}

void CrearBala(bala& Bala, nave& jugador, int vel) {
	//Se  inicializan los valores de las balas la posici�n en x y y, la velocidad y estado
	Bala.x = jugador.x;
	Bala.y = jugador.y - 10;
	Bala.velocidad = vel;
	Bala.estado = true;
}

void pintarBala(bala& Bala, ALLEGRO_BITMAP*& BalaE) {
	//Se define la funci�n pintarBala la cual se encarga de determinar si el estado de la bala es activo o inactivo para as� pintarla 
	//Tambi�n define que si la bala tiene una posici�n fuera de los l�mites de la pantalla esta se inactive
	if (Bala.estado == false) { // no existe bala
		return;
	}
	if (Bala.y <= 0 || Bala.y >= ResY) { // si se sale de los limites de pantalla no hace nada
		Bala.estado = false;
		return;
	}
	al_draw_bitmap(BalaE, Bala.x - 2, Bala.y - 12, NULL); // se dibuja bala en pantalla
	Bala.y = Bala.y - Bala.velocidad;//Hace que la bala avance. Ejemplo, si la velocidad de la bala es 5 entonces se mueve 5 hacia arriba porque pantalla esta invertida
}

int naveRand(PtrNave& Enemigos, int activos[57]) {
	//Se define la funci�n naveRand que define una parte del array dependiendo de cuantas naves queden disponibles y cuales est�n activas
	//luego se elije alguna nave activa disponible random y con este retorno se eligir� cual nave disparar�
	PtrNave Aux;
	Aux = Enemigos;
	int num = 0;
	while (Aux != NULL) {
		if (Aux->estado == true) { // entre enemigos activo elije uno random
			activos[num] = Aux->codigo;
			num++;
		}
		Aux = Aux->Siguiente;
	}
	activos[55] = num;
	if (num == 0) {
		num = 57;
	}
	Aux = NULL;
	srand(time(NULL));
	int random = rand() % num;//Define numero random
	return activos[random];//elige de forma pseudoaleatoria una nave activa
}

void disparoEnemigo(PtrNave& Enemigos, bala& Bala, int vel, nave& jugador, int activos[57], ALLEGRO_SAMPLE* Sonido) {
	//Se crea la funci�n disparo Enemigo la cual va a crear el disparo y movimiento de la bala desde la nave random activa
	if (Bala.estado != false) {
		return;//Si la bala est� desactivada, no se pinta
	}
	PtrNave Aux;
	Aux = Enemigos;
	int codEnemigo = naveRand(Enemigos, activos);//Se define el codigo aleatorio de la nave que disparar� 
	while (Aux != NULL) {
		if (Aux->codigo == codEnemigo) {
			CrearBala(Bala, *Aux, vel);//Se crea la bala y se reproduce el sonido de disparo
			al_play_sample(Sonido, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

			return;
		}
		Aux = Aux->Siguiente;
	}
	Aux = NULL;
}

void colisionEscudo(escudo Arreglo[30], bala& Bala) {
	//Se crea la funci�n colisionEscudo que detecta mediante un hitbox si la bala enemiga traslapa el hitbox
	for (int i = 0; i < 24; i++) {
		if (Arreglo[i].estado == true) { // compara dos cuadrados de pixeles y si en la parte en que se intersecan los cuadrados sera un hitbox, entonces tienen una colision
			if (Bala.velocidad < 0) {
				if ((Bala.x >= Arreglo[i].X) && (Bala.x <= Arreglo[i].X + 22) && (Bala.y + 12 >= Arreglo[i].Y + 19) && (Bala.y - 12 <= Arreglo[i].Y)) {
					//Si se traslapa se le suma una vida al escudo(Esto representa un da�o y con este contador se cambiar� el bitmap)
					//Se eimina la bala
					Arreglo[i].vidas++; // se suman las vidas que va a ser el da�o a cada segemtno del escudo
					Bala.estado = false;
					Bala.y = -50;
					if (Arreglo[i].vidas >= 4) Arreglo[i].estado = false;
					//Si las vidas superan a 4, se establece ese segmento como destruido

				}
			}
			else {
				if ((Bala.x >= Arreglo[i].X) && (Bala.x <= Arreglo[i].X + 22) && (Bala.y + 12 >= Arreglo[i].Y) && (Bala.y - 12 <= Arreglo[i].Y + 19)) {
					//Si se traslapa se le suma una vida al escudo(Esto representa un da�o y con este contador se cambiar� el bitmap)
					//Se eimina la bala
					Arreglo[i].vidas++;
					Bala.estado = false;
					Bala.y = -50;
					if (Arreglo[i].vidas >= 4) Arreglo[i].estado = false;
					//Si las vidas superan a 4, se establece ese segmento como destruido
				}
			}
		}
	}
}

// Inicializa la creacion de los escudos
void crearEscudo(escudo Arreglo[30], int x, int y) {
	//Se define la funci�n crearEscudo que funciona como un iniciador de cada segmento de escudo
	int r = 0;
	char ArregloEscudos[3][22] = {
		//Se crea una matriz que funcionar� para determinar qu� areas se van a inicializar como segmentos de escudo
		// Para cada una de esas letras es donde va a estar cada parte del escudo
		"ABC   ABC   ABC   ABC",
		"DEF   DEF   DEF   DEF",
	};
	// Se asignan las diferentes caracteristicas para cada una de las partes del escudo para cada segmento
	for (int i = 0; i < 21; i++) {//Se  recorre toda la matriz
		for (int j = 0; j < 2; j++) {
			if (ArregloEscudos[j][i] != ' ') {
				//Se definen las posiciones en x y y  segun el display para cada segmento del escudo y se activa cada uno
				Arreglo[r].Y = (ResY - 200) + j * 19;
				Arreglo[r].X = (x / 2) - 350 + 141 + (i * 20);
				Arreglo[r].estado = true; // el segmento puede recibir mas colisiones
				//Se definen las propiedades de cada segmento dependiendo de su valor en la matriz
				//as asignaciones se definen en cada estructura del arreglo de estructuras
				// El codigo ayuda en el dibujo de los bitmaps y este va avanzando en cada if
				if (ArregloEscudos[j][i] == 'A') {
					Arreglo[r].codigo2 = 0;
					Arreglo[r].codigo = 0;
					Arreglo[r].vidas = 0; // va sumando en vez de disminuir
					Arreglo[r].ancho = 22;

				}
				if (ArregloEscudos[j][i] == 'B') {
					Arreglo[r].codigo2 = 0;
					Arreglo[r].codigo = 22;
					Arreglo[r].vidas = 0;
					Arreglo[r].ancho = 20;
				}
				if (ArregloEscudos[j][i] == 'C') {
					Arreglo[r].codigo2 = 0;
					Arreglo[r].codigo = 42; // resolution de imagen del bitmap que estamos utilizando 
					Arreglo[r].vidas = 0;
					Arreglo[r].ancho = 22;
				}
				if (ArregloEscudos[j][i] == 'D') {
					Arreglo[r].codigo2 = 19;
					Arreglo[r].codigo = 0; // vuelve a cero porque 
					Arreglo[r].vidas = 0;
					Arreglo[r].ancho = 22;
				}
				if (ArregloEscudos[j][i] == 'E') {
					Arreglo[r].codigo2 = 19;
					Arreglo[r].codigo = 22;
					Arreglo[r].vidas = 0;
					Arreglo[r].ancho = 20;
				}
				if (ArregloEscudos[j][i] == 'F') {
					Arreglo[r].codigo2 = 19;
					Arreglo[r].codigo = 42;
					Arreglo[r].ancho = 22;
					Arreglo[r].vidas = 0;
				}
				r++; // avanza en el arreglo
			}

		}
	}
}

void dibujarEscudos(escudo Arreglo[30], ALLEGRO_BITMAP* ESCUDO) {
	//Se dibuja cada segmento del escudo mediante regiones de bitmaps
	for (int i = 0; i < 24; i++) { // son 24 por la matriz de los escudos
		if (Arreglo[i].estado == true) {
			// Se agarra la imagen del escudo y se va pintando en regiones para que asi vaya dibujandose y va depender de las vidas que tenga
			al_draw_bitmap_region(ESCUDO, Arreglo[i].codigo, 38 * Arreglo[i].vidas + Arreglo[i].codigo2, Arreglo[i].ancho, 19, Arreglo[i].X, Arreglo[i].Y, 0); // Para cada letra se va a dibujar el pedacito
		}
	}

}

void CrearArchivo(char* puntaje, char* nombre)//Se crea la funci�n CrearArchivo que guarda el nombre y el puntaje en un archivo en memoria secundaria
{
	FILE* archivo;
	archivo = fopen("resultados.txt", "a");

	if (NULL == archivo) {
		fprintf(stderr, "No se pudo crear archivo %s.\n", "resultados.txt");
		exit(-1);
	}
	else {
		fprintf(archivo, "Nombre:%s\n", nombre);
		fprintf(archivo, "Puntaje: %s\n", puntaje);
		fprintf(archivo, "\n\n");
	}
	fclose(archivo);
}

// el parametro inicio sirve para ir bajando y viendo los resultados. Ese parametro lo va a determinar el resultado a partir del cual se va a imprimir en pantalla y solo muestra 4
void CargarArchivo(int x, int y, ALLEGRO_FONT* fuente, int inicio)//Se carga el archivo en memoria secundaria a pantalla con el puntaje y nombre escritos en el display
{
	char nombre[40];
	char puntaje[10];
	int i = 0;
	int e = 0;
	FILE* archivo;
	archivo = fopen("resultados.txt", "r"); // lee el archivo .txt
	if (archivo != NULL) {
		while (!feof(archivo)) {
			fscanf(archivo, "Nombre:%s\n", nombre); // busca nombre y puntaje
			fscanf(archivo, "Puntaje: %s\n", puntaje);
			if (inicio <= i && i < inicio + 4) { // se muestran en pantalla solo 4 resultados por ventana
				al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (300.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_RIGHT, "Nombre: ");
				al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (300.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_LEFT, nombre);
				al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (330.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_RIGHT, "Puntaje: ");
				al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (330.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_LEFT, puntaje);
				e++;
			}
			i++;
		}
		fclose(archivo);
	}
}