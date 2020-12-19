#include "GameCharacter.h"
#include "Game.h"

GameCharacter::GameCharacter(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC)
	:GameObject(posAct, casillaWidth, casillaHeight, gameC), posIni(posIniC), dir(dirC), texture(textureC), coorTexture(coorTextureC) {
	
}


//se crea un gameObject vacío,ya que primero va la construcotra del padre y luego se le van cambiando los valores al leerlos.
GameCharacter::GameCharacter(ifstream& file, Game* gameC): GameObject(Point2D(0,0), 0, 0, gameC) {
	
	//recibe el flujo del archivo que se quiere leer, además necesita una referencia a Game ya que no hay manera de guardarlo en el archivo,
	//Además gracias a game se puede acceder a la textura 
	int posY, posX, posIniY, posIniX, dirY, dirX , coorY, coorX;

	//segun va leyendo va dando valor a las variables
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


//no hace nada ya que es el game eel que se encarga de borrar la memoria dinámica,
//se podría borrar las texturas pero daría error al intentar borrarlas de nuevo en Game
GameCharacter::~GameCharacter() {
}

void GameCharacter::saveToFile(std::ofstream& archivoDeGuardado) {
	archivoDeGuardado << pos.GetY() << " " << pos.GetX() << " " << width << " " << height << " " << posIni.GetY() << " " << posIni.GetX()
		<< " " << dir.GetY() << " " << dir.GetX() << " " << coorTexture.GetY() << " " << coorTexture.GetX() << " " ;
}