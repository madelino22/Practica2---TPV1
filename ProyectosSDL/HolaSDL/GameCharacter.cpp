#include "GameCharacter.h"


GameCharacter::GameCharacter(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC)
	:GameObject(posAct, casillaWidth, casillaHeight, gameC), posIni(posIniC), dir(dirC), texture(textureC), coorTexture(coorTextureC) {
	//texture = gameC->getTexture();
	setItList(it);
}