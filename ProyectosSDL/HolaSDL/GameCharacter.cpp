#include "GameCharacter.h"


GameCharacter::GameCharacter(Game* gamePunt, int casillaHeight, int casillaWidth, Point2D posAct, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC) {
	game = gamePunt;
	height = casillaHeight;
	width = casillaWidth;
	pos = posAct;
	posIni = posIniC;
	dir = dirC;
	texture = textureC;
	setItList(it);
}