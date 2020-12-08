#pragma once
#include"Vector2D.h"
#include "SDL.h"


class Game;

class GameObject
{
protected:
	Point2D pos;
	int width;
	int height;
	Game* game;

public:
	virtual void render() const {};
	virtual void update();
	virtual SDL_Rect getDestRect();
	
	//No crea memoria dinámica por lo que no debería borrar nada
	virtual ~GameObject();

};

