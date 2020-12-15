#pragma once
#include "Ghost.h"
class SmartGhost: public Ghost
{
	const int tiempoChild = 100;
	const int tiempoAdult = 300;
	const int tiempoDead = 350;
	const int tiempoQuerentine = 100;
private:

	void handleState();

protected:
	enum State{Child, Adult, Quarentine, Dead};
	int edad;
	int capMov = 14;
	State state;


public:
	SmartGhost(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC, int edad);
	SmartGhost(ifstream& file, Game* gameC);

	
	int getCooldown() const { return cooldown; };

	virtual void saveToFile(std::ofstream& file);
	virtual void update();
	virtual void render() const;
};

