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

	destRect.x = posAct.GetX() * casillaW;
	destRect.y = posAct.GetY() * casillaH;
	destRect.h = casillaH;
	destRect.w = casillaW;


	//para saber que fantasma(color) hay que renderizar
	switch (color){
	case 0:
		texture->renderFrame(destRect, 0, 0);
		break;
	case 1:
		texture->renderFrame(destRect, 0, 2);
		break;
	case 2:
		texture->renderFrame(destRect, 0, 6);
		break;
	case 3:
		texture->renderFrame(destRect, 0, 8);
		break;
	default:
		throw "El color del fantasma está mal";
		break;
	}
	
}

//Aqui hay que hacer que decida de manera aleatoria donde ir
void Ghost::update() {
	int nCols = game->GetNCols();
	int nFils = game->GetNFils();
	
	Vector2D posibles[3];
	Vector2D dirPos = dir;
	int tam = 0;
	for (int x = 0; x < 3; x++) {
		if (game->NextCell(dirPos, posAct)) {
			posibles[tam] = dirPos;
			tam++;
		}
		//primero checkeo si en la direcion que está yendo se puede,
		//luego una de las de al lado(perpendicular) y al final la inversa a la última(la que era perpendicular)
		//así siempre se chequean las direcciones que no es marcha atrás, si ninguna es posible, no queda otra que ir hacia atrás
		if (x == 0) dirPos.Perpendicular();
		else if (x == 1) dirPos.Invierte();
	}

	if (tam == 0) {
	    dir.Invierte();
	}
	
	else {
		//selecciona al azar de entre una de las posibles
		int nuevaDir = rand() % tam;
		dir = posibles[nuevaDir];
	}
	
	comer(game->getPacManPosAct());
	//la suma de nCols y nFils dentro del parentesis es para que al ir en velocidad negativa y llegue al borde aparezca al otro lado tmb
	posAct.SetX((posAct.GetX() + dir.GetX() + nCols) % nCols);
	posAct.SetY((posAct.GetY() + dir.GetY() + nFils) % nFils);
	comer(game->getPacManPosAct());
	//Come antes y depués del movimiento para evitar que se crucen sin interactuar
}