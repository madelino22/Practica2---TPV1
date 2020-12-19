#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include <iostream>
#include "Texture.h"
#include "Vector2D.h"
#include "GameCharacter.h"
using namespace std;

using uint = unsigned int;


class Pacman: public GameCharacter
{

private:

	
	Vector2D  newDir;
	
	bool eating;
	int eatingDistance;//la duración/distancia del modo caza del pacman
	void comerAlimento();

public:
	Pacman(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC);
	Pacman(ifstream& file, Game* gameC);
	virtual ~Pacman() {}

	Point2D GetPosAct() const { return pos; }
	Point2D GetPosIni() const { return posIni; }
	//devuelve una variable para saber si está comiendo, es util para qeu los fantamas los sepan y asi puedan comer o ser comidos
	bool GetEating() const { return eating; }


	void SetPosAct(Point2D newpos) { pos = newpos; }
	void SetDir(int x, int y) { dir.SetX(x); dir.SetY(y); }
	void SetNewDir(int x, int y) { newDir.SetX(x); newDir.SetY(y); }

	virtual void saveToFile(std::ofstream& file);


	virtual void render() const;
	virtual void update();
	void handleEvents(const SDL_Event& event);
};

