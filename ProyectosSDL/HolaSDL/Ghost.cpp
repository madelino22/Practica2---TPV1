#include "Ghost.h"
#include "Game.h"



bool Ghost::comer(Point2D posPacMan) {
	bool comido = false;
	if (posPacMan == posAct) {
		if (!game->pacmanEating()) {
			//Si los fantasmas son los que comen
		    //invocan método pacman de reaparecer del pacman
		    //y quitan una vida
			game->pacManRespawn();
			comido = true;
		}
		//Si son comidos simplemente vuelven a la posición inicial
		else {
			morir();
		}
		
	}

	return comido;
}


void Ghost::morir() {
	posAct = posIni;
}
void Ghost::render() const {

	int casillaH = WIN_HEIGHT / game->GetNFils();
	int casillaW = WIN_WIDTH / game->GetNCols();

	//creacion e inicializacion del rectángulo destino
	SDL_Rect destRect;

	destRect.x = posAct.GetX();
	destRect.y = posAct.GetY();
	destRect.h = casillaH;
	destRect.w = casillaW;
	texture->renderFrame(destRect, 0, color*2);
	
}

//Aqui hay que hacer que decida de manera aleatoria donde ir
void Ghost::update() {
	int nCols = game->GetNCols();
	int nFils = game->GetNFils();
	
	Vector2D posibles[3];
	Point2D casillasPosibles[3];
	Vector2D dirPos = dir;
	int tam = 0;

	SDL_Rect rect;
	rect.x = posAct.GetX();
	rect.y = posAct.GetY();
	//Esto está pinchado por código, cuando se haga polimorfismo se podrá hacer bien
	rect.w = 28;
	rect.h = 20;


	for (int x = 0; x < 3; x++) {
		Point2D casillaDestino;
		if (game->tryMove(rect, dirPos, casillaDestino)) {
			posibles[tam] = dirPos;
			casillasPosibles[tam] = casillaDestino;
			tam++;
		}
		//primero checkeo si en la direcion que está yendo se puede,
		//luego una de las de al lado(perpendicular) y al final la inversa a la última(la que era perpendicular)
		//así siempre se chequean las direcciones que no es marcha atrás, si ninguna es posible, no queda otra que ir hacia atrás
		if (x == 0) dirPos.Perpendicular();
		else if (x == 1) dirPos.Invierte();
	}

	comer(game->getPacManPosAct());

	if (tam == 0) {
		Point2D destino;
	    dir.Invierte();
		game->tryMove(rect, dir, destino);
		posAct = destino;
	}
	
	else {
		//selecciona al azar de entre una de las posibles
		int nuevaDir = rand() % tam;
		dir = posibles[nuevaDir];
		posAct = casillasPosibles[nuevaDir];
	}
	
	
	comer(game->getPacManPosAct());
	//Come antes y depués del movimiento para evitar que se crucen sin interactuar

	
}