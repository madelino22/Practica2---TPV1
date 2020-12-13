#include "GameCharacter.h"
#include "Game.h"

GameCharacter::GameCharacter(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC)
	:GameObject(posAct, casillaWidth, casillaHeight, gameC), posIni(posIniC), dir(dirC), texture(textureC), coorTexture(coorTextureC) {
	//texture = gameC->getTexture();
	setItList(it);
}


GameCharacter::GameCharacter(ifstream& file, Game* gameC): GameObject(Point2D(0,0), 0, 0, gameC) {
	
	int posY, posX, posIniY, posIniX, dirY, dirX , coorY, coorX;

	file >> posY >> posX >> width >> height >> posIniY >> posIniX >> dirY >> dirX >> coorY >> coorX;

	texture = game->getTexture("character");

	pos.SetY(posY);
	pos.SetX(posX);

	posIni.SetY(posIniY);
	posIni.SetX(posIniX);

	dir.SetY(dirY);
	dir.SetX(dirX);

	coorTexture.SetY(coorY);
	coorTexture.SetX(coorX);

}



void GameCharacter::saveToFile(std::ofstream& archivoDeGuardado) {
	archivoDeGuardado << pos.GetY() << " " << pos.GetX() << " " << width << " " << height << " " << posIni.GetY() << " " << posIni.GetX()
		<< " " << dir.GetY() << " " << dir.GetX() << " " << coorTexture.GetY() << " " << coorTexture.GetX() << " " ;
}