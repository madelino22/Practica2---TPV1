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
		game->eraseGhost(itList);
	else if (edad >= tiempoAdult) state = Dead;
	else if (edad >= tiempoChild) state = Adult;
    else state = Child;
	
	

}

void SmartGhost::saveToFile(std::ofstream& file) {
	file << 2 << " ";//este cero es para saber que es un fantasma inteligente
	Ghost::saveToFile(file);
	file << edad << "\n";
}


void SmartGhost::movimiento(int posRelPacmanY, int posRelPacmanX) {
	Point2D newPos;
	int x = posRelPacmanX;
	int y = posRelPacmanY;
	if (!game->pacmanEating()) {
		if (x < 0 && game->tryMove(getDestRect(), Point2D(0, -1), newPos)) //Está a la izquierda
		{
			pos = newPos;
		}
		else if (x > 0 && game->tryMove(getDestRect(), Point2D(0, 1), newPos)) //Está a la derecha
		{
			pos = newPos;
		}

		if (y < 0 && game->tryMove(getDestRect(), Point2D(-1, 0), newPos)) //Está arriba
		{
			pos = newPos;
		}
		else if (y > 0 && game->tryMove(getDestRect(), Point2D(1, 0), newPos)) //Está abajo
		{
			pos = newPos;
		}
	}
	else {
		if (x < 0 && game->tryMove(getDestRect(), Point2D(0, 1), newPos)) //Está a la izquierda
		{
			pos = newPos;
		}
		else if (x > 0 && game->tryMove(getDestRect(), Point2D(0, -1), newPos)) {
			pos = newPos;
		}

		if (y < 0 && game->tryMove(getDestRect(), Point2D(1, 0), newPos)) //Está a la izquierda
		{
			pos = newPos;
		}
		else if (y > 0 && game->tryMove(getDestRect(), Point2D(-1, 0), newPos)) {
			pos = newPos;
		}
	}
}



void SmartGhost::update() {
	handleState();
	edad++;
	

	
	int x, y = 0;
	
	if (state == Child || state == Adult) {

		if (game->distanciaAlPacman(pos, x, y) < 175) {
			
			if (state == Adult) {
				comer(game->getPacManPosAct());
			}

			movimiento(y, x);


			if (state == Adult) {
				comer(game->getPacManPosAct());
			}


		}
		//si la distancia es mayor se hace el moviemiento normal de los fantasmas, además como esta lejos no importa que detecte el comer
		else {
			Ghost::update();
		}
	}
	
	

	game->checkColisionFantasmas(this);
	
}



void SmartGhost::render() const{
	SDL_Rect destRect = getDestRect();
	
	//Esto funciona ya qeu es el destRect de arriba el que se le pasa a tryMove
	//la modificacion del destRecto SOLO se hace aqui, en el render, por lo que para el juego sigue
	//midiendo lo que mide un fantasma grande
	if (state == Child) {
		
		destRect.x = destRect.x + destRect.w * 1 / 3;
		destRect.y = destRect.y + destRect.h * 1 / 3;
		destRect.h = destRect.h * 2 / 3;
		destRect.w = destRect.w * 2 / 3;
	}
	
	texture->renderFrame(destRect, 0, coorTexture.GetX());
	
}
