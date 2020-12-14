#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include <iostream>
#include "Texture.h"
#include "Pacman.h"
#include "GameMap.h"
#include <fstream>
#include "Ghost.h"
#include <array>
#include <list>
#include <string>


using namespace std;

using uint = unsigned int;

struct Texturas {
	string file[4]{ "..\\Imagenes\\wall3.png", "..\\Imagenes\\characters1.png", "..\\Imagenes\\burguer1.png", "..\\Imagenes\\food2.png" };
	int fils[4]{ 1, 4, 1, 1 };
	int cols[4]{ 1, 14, 1, 1 };
};

const uint WIN_WIDTH = 800;
const uint WIN_HEIGHT = 600;
const uint NUM_TEXTURES = 4;
const uint32_t FRAME_RATE = 15;

const int avanceEnX = 1;
const int avanceEnY = 1;

class Game
{
	
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	// uint winWidth, winHeight; // También podrían estar aquí
	Pacman* pacman = nullptr;
	GameMap* mapa = nullptr;
	list<Ghost*> ghosts;
	list<GameObject*> objetos;
	bool exit = false;
	
	array<Texture*, NUM_TEXTURES> textures;

	int objetosCharacter;
	int nivel;

	int vidas;
	int comida;
	bool ganado, pausado;

	void LeeMapa();
	void loadLevelFile(string file);
	void destruccionesCambioNivel();


public:
	Game();
	~Game();

	int GetNFils() const;
	int GetNCols() const;
	bool NextCell(const Vector2D& dir,const Vector2D& pos) const;
	bool tryMove(const SDL_Rect& rect, Vector2D dir, Point2D& newPos);

		//El rect es el  rectangulo actual del gameObject qu ese quiere mover, sustituye a pos de NextCel
	

	MapCell contenidoCelda(int y, int x) const{
		Point2D coordenadas = SDLPointToMapCoords(Point2D(y, x));
		
		return mapa->celdasMapa[coordenadas.GetY()][coordenadas.GetX()];
	}



	//Para que los fantasmas sepan donde está el pacman para saber si hay que comerselo o no
	Point2D getPacManPosAct() const { return pacman->GetPosAct(); }
	//Devuelve un punto en [y,x] con las cordenadas(en piexeles) de la esquina superior de donde deberia ir pintado el ese cuadrado
	Point2D mapCordsToSDLPoint(Point2D coords) const{ return Point2D(coords.GetY() * mapa->height, coords.GetX() * mapa->width); };
	//Devuelve un punto en [y,x] con las cordenadas(en casilla) de la esquina superior de donde deberia ir pintado el ese cuadrados
	Point2D SDLPointToMapCoords(Point2D mapCoor) const { return Point2D(mapCoor.GetY() / mapa->height, mapCoor.GetX() / mapa->width); }
    
	//Este método es para indicar a los fantasmas si el pacman está comiendo, para que sepan si tienen que comerselo o ser comidos
	bool pacmanEating() const { return pacman->GetEating(); };

	SDL_Rect GetPacmanRect() const {
		return pacman->getDestRect();
	}

	//lleva al pacman a la posición inicial y resta una vida, si no le quedan vidas se encarga de finalizar la partida
	void pacManRespawn();
	
	//al comer una bola amarilla se invoca a este método
	void Comida() {
		comida--;
	};

	//hace que la casilla que tiene los parámetros x e y sea vacía, es para cuando se come vitamina o comida
	void EmptyCell(int y, int x) {
		Point2D coords = SDLPointToMapCoords(Point2D(y, x));
		mapa->celdasMapa[coords.GetY()][coords.GetX()] = Empty;
	};

	Texture* getTexture(string name);

	void save();
	void loadSavedGame();

	void run();
	void render() const;
	void handleEvents();
	void update();

};

