#pragma once
#include <string>


#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include "Texture.h"
#include <iostream>
#include "GameObject.h"








using uint = unsigned int;
using namespace std;

const int MARGENX = 0;
const int MARGENY = 0;


enum MapCell{Empty, Wall, Food, Vitamins};


class GameMap: public GameObject
{
	friend class Game;
private:
	int fils, cols;
	MapCell** celdasMapa;//Es un array dinámico de punteros a MapCell
	Texture* textureWall = nullptr;
	Texture* textureVit = nullptr;
	Texture* textureFood = nullptr;

	
    
public:
	GameMap(Point2D posC, int wC, int hC, Game* game, Texture* textMap, Texture* textVit, Texture* textFood, int nFils, int nCols);
	//virtual ~GameMap();

	bool IntersectsWall(SDL_Rect rect);
	SDL_Rect getDestRect();

	virtual void render() const;
	//update no hce nada
	virtual void update() {};

};

