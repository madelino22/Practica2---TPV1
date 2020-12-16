#pragma once
#include "Ghost.h"
class SmartGhost: public Ghost
{
	const int tiempoChild = 400;
	const int tiempoAdult = 1500;
	const int tiempoDead = 1700;
private:

	void handleState();
	void movimiento(int posRelPacmanY, int posRelPacmanX);

protected:
	enum State{Child, Adult, Dead};//la cuarentena se mide mediante la variable cooldown de ghost
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

