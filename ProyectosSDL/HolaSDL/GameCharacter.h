#pragma once
#include "GameObject.h"
#include "Texture.h"
#include <list>
#include <iostream>


class GameCharacter: public GameObject
{
protected:
	Point2D posIni;
	Vector2D dir;
	Texture* texture = nullptr;
	Point2D coorTexture;
	list<GameObject*>::iterator itList;


	
	

public:
	
	GameCharacter(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC);
	GameCharacter(ifstream& file, Game* gameC);

	//devuelve el iterador que apunta al objeto en objects
	void setItList(list<GameObject*>::iterator& itPasar) {
		itList = itPasar;
	};

	virtual void saveToFile(std::ofstream& file);
	virtual void render() const = 0;
	virtual void update() = 0;

	virtual ~GameCharacter();
};

