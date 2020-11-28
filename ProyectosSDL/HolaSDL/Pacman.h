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

	Point2D GetPosAct() const { return posAct; }
	Point2D GetPosIni() const { return posIni; }
	bool GetEating() const { return eating; }


	void SetPosAct(Point2D newpos) { posAct = newpos; }
	void SetDir(int x, int y) { dir.SetX(x); dir.SetY(y); }
	void SetNewDir(int x, int y) { newDir.SetX(x); newDir.SetY(y); }




	void render() const;
	void update();
	void handleEvents(const SDL_Event& event);
};

