#pragma once
#include "GameObject.h"
#include "Texture.h"
#include <list>


class GameCharacter: public GameObject
{
protected:
	Point2D posIni;
	Vector2D dir;
	Texture* texture = nullptr;
	Point2D coorTexture;
	list<GameObject>::iterator it;


	
	void setItList(list<GameObject>::iterator& itPasar);//Este método hay que hacerle
	GameCharacter(Game* gamePunt, int casillaHeight, int casillaWidth, Point2D posAct, Point2D posIniC, Vector2D dirC, Texture* textureC,  Point2D coorTextureC);

	
};

