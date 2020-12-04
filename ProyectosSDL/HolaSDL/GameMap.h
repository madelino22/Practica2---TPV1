#pragma once
#include <string>


#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include "Texture.h"
#include <iostream>







using uint = unsigned int;
using namespace std;

const int MARGENX = 0;
const int MARGENY = 0;


enum MapCell{Empty, Wall, Food, Vitamins};
class Game; // para que pueda guardar el juego y no haya inclusiones circulares


class GameMap
{
	friend class Game;
private:
	int fils, cols;
	MapCell** celdasMapa;//Es un array din�mico de punteros a MapCell
	Texture* textureWall = nullptr;
	Texture* textureVit = nullptr;
	Texture* textureFood = nullptr;
	Game* game;
	int casillaH;
	int casillaW;
    
public:
	GameMap(){};
	GameMap(int fils, int cols, Game* game, Texture* textMap, Texture* textVit, Texture* textFood);
	~GameMap();

	bool intersectsWall(SDL_Rect rect);
	SDL_Rect getDestRect();

	void render() const;

};

