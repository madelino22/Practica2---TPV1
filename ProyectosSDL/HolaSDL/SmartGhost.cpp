#include "SmartGhost.h"
#include "Game.h"


SmartGhost::SmartGhost(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC, int edadC) :
	Ghost(posAct, casillaWidth, casillaHeight, gameC, posIniC, dirC, textureC, coorTextureC), edad(edadC) {
	state = Child;
}

SmartGhost::SmartGhost(ifstream& file, Game* gameC) : Ghost(file, gameC) {
	file >> edad;
}


void SmartGhost::handleState() {


	if (edad >= tiempoDead)
		game->eraseGhost(this);
	else if (edad >= tiempoAdult) state = Dead;
	else if (edad >= tiempoChild) state = Adult;
    else state = Child;
	
	

}

void SmartGhost::saveToFile(std::ofstream& file) {
	file << 2 << " ";//este cero es para saber que es un fantasma inteligente
	Ghost::saveToFile(file);
	file << edad << "\n";
}

void SmartGhost::update() {
	handleState();
	edad++;
	if (edad < tiempoAdult) {
		Ghost::update();
	}

	if (edad > tiempoAdult) {
		//muere
	}
}

void SmartGhost::render() const{
	SDL_Rect destRect = getDestRect();
	if (edad < tiempoChild) {
		
		destRect.x = destRect.x + destRect.w * 1 / 3;
		destRect.y = destRect.y + destRect.h * 1 / 3;
		destRect.h = destRect.h * 2 / 3;
		destRect.w = destRect.w * 2 / 3;
	}
	
	texture->renderFrame(destRect, 0, coorTexture.GetX());
	
}
