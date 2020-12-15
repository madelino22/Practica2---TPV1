#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include <iostream>
#include "Texture.h"
#include "Vector2D.h"
#include <cstdlib>
#include "GameCharacter.h"
using namespace std;

using uint = unsigned int;

class Game;


class Ghost: public GameCharacter
{

protected:

	int capMov;
	int cooldown;

	bool comer(Point2D posPacMan);//de momento es bool pero no se usa
	void morir();

public:
	Ghost(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC);
	Ghost(ifstream& file, Game* gameC);
	virtual ~Ghost() {}

	int getCooldown() const { return cooldown; };
	void setCooldown(int x) { cooldown = x; };
    bool pacmanCollison();
	void EscribePosicion(){ cout << pos.GetY() << " " << pos.GetX(); }

	virtual void saveToFile(std::ofstream& file);

	virtual void render() const;
	virtual void update();
	void handleEvents(const SDL_Event& event);
};

