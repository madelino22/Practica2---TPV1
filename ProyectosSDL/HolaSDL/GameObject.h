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
	GameObject(Point2D posC, int wC, int hC, Game* gameC) : pos(posC), width(wC), height(hC), game(gameC) {};
	//m�todos abstractos
	virtual void render() const = 0;
	virtual void update() = 0;
	virtual SDL_Rect getDestRect() const;
	
	//No crea memoria din�mica por lo que no deber�a borrar nada
	virtual ~GameObject() {};

};

