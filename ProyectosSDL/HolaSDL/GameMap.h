#pragma once
#include <string>


#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include "Texture.h"
#include <iostream>







using uint = unsigned int;
using namespace std;


enum MapCell{Empty, Wall, Food, Vitamins};
class Game; // para que pueda guardar el juego y no haya inclusiones circulares


class GameMap
{
	friend class Game;
private:
	int fils, cols;
	MapCell** celdasMapa;//Es un array dinámico de punteros a MapCell
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

	void render() const;

};

