#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include <iostream>
#include "Texture.h"
#include "Vector2D.h"

using namespace std;

using uint = unsigned int;
class Game; // para que pueda guardar el juego y no haya inclusiones circulares


class Pacman
{
	friend class Game;

private:

	Point2D posAct, posIni;
	Vector2D dir, newDir;
	Game* game;

	Texture* texture = nullptr;
	
	bool eating;
	int eatingDistance;//la duración/distancia del modo caza del pacman
	void comerAlimento();

public:
	Pacman() {}
	Pacman(Point2D pIni,Game* g, Texture* t) :
		posIni(pIni), posAct(pIni), game(g), texture(t),  dir(Vector2D(1,0)) , newDir(Vector2D(1, 0)), eating(false), eatingDistance(15) {}
	~Pacman() {}

	void render() const;
	void update();
	void handleEvents(const SDL_Event& event);
};

