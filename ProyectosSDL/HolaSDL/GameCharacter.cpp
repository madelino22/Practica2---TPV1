#include "GameCharacter.h"
#include "Game.h"

GameCharacter::GameCharacter(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC)
	:GameObject(posAct, casillaWidth, casillaHeight, gameC), posIni(posIniC), dir(dirC), texture(textureC), coorTexture(coorTextureC) {
	//texture = gameC->getTexture();
	setItList(it);
}

/*
GameCharacter::GameCharacter(ifstream& file, Game* gameC, Texture* textureC, Point2D coorTextureC) {
	int posY, posX, posIniY, posIniX, dirY, dirX , w, h;

	Point2D posC, posIniC;
	Vector2D dirC;

	posC.SetY(posY);
	posC.SetX(posX);

	posIniC.SetY(posIniY);
	posIniC.SetX(posIniX);

	dirC.SetY(dirY);
	dirC.SetX(dirX);

	GameObject(posC, w, h, gameC);
	file >> posY >> posX >> w >> h >> posIniY >> posIniX >> dirY >> dirX;




}
*/


void GameCharacter::saveToFile(std::ofstream& archivoDeGuardado) {
	archivoDeGuardado << pos.GetY() << " " << pos.GetX() << " " << width << " " << height << " " << posIni.GetY() << " " << posIni.GetX() << " " << dir.GetY() << " " << dir.GetX() << " ";
}