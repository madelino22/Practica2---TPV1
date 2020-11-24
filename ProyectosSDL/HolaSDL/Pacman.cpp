#include "Pacman.h"
#include "Game.h"



void Pacman::render() const {
	
	int casillaH = WIN_HEIGHT / game->GetNFils();
	int casillaW = WIN_WIDTH / game->GetNCols();
	SDL_Rect destRect;

	destRect.x = posAct.GetX() * casillaW;
	destRect.y = posAct.GetY() * casillaH;
	destRect.h = casillaH;
	destRect.w = casillaW;

	//si está en modo caza tiene distinto sprite
	if(!eating)texture->renderFrame(destRect, 0, 10);
	else {texture->renderFrame(destRect, 0, 11);}
}


void Pacman::comerAlimento() {
	//Pueden ser o vitaminas o comida
	if (game->contenidoCelda(posAct.GetY(), posAct.GetX()) == Food) {
		//si es comida la elmina del mapa y la resta del contador de comida restante, cuando este contador llegue a 0 el juego se acaba
		game->Comida();
		game->EmptyCell(posAct.GetY(), posAct.GetX());
	}
	else if (game->contenidoCelda(posAct.GetY(), posAct.GetX()) == Vitamins) {
		//si es vitamina se pone en modo caza y se elimina esa vitamina del mapa, no hace falta recogerlas todas para ganar
		eating = true;
		eatingDistance = 25;
		game->EmptyCell(posAct.GetY(), posAct.GetX());
	}
}

void Pacman::update() {
	int nCols = game->GetNCols();
	int nFils = game->GetNFils();
	//el primer if checkea si se puede ir a la última posición indicada por la pulsacíon, si se puede se cambia la dirección, si no sigue yendo a la que estaba yendo

	
	if (game->NextCell(newDir, posAct))dir = newDir;
	if (game->NextCell(dir, posAct)) {
		//la suma de nCols y nFils dentro del parentesis es para que al ir en velocidad negativa y llegue al borde aparezca al otro lado tmb
		posAct.SetX((posAct.GetX() + dir.GetX() + nCols) % nCols);
		posAct.SetY((posAct.GetY() + dir.GetY() + nFils) % nFils);
	}

	comerAlimento();

	if (eating) {
		eatingDistance--;
		if (eatingDistance <= 0) {
			eating = false;
			eatingDistance = 0;//por si acaso se ha puesto en negativos
		}
	}
	
}

void Pacman::handleEvents(const SDL_Event& event) {
	if (event.type == SDL_KEYDOWN) {
		SDL_Keycode key = event.key.keysym.sym;
		if (key == SDLK_UP || key == SDLK_DOWN || key == SDLK_RIGHT || key == SDLK_LEFT ){
			if (key == SDLK_UP) {
				newDir.SetX(0);
				newDir.SetY(-1);
			}
			else if (key == SDLK_DOWN) {
				newDir.SetX(0);
				newDir.SetY(1);
			}
			else if (key == SDLK_RIGHT) {
				newDir.SetX(1);
				newDir.SetY(0);
			}
			else if (key == SDLK_LEFT) {
				newDir.SetX(-1);
				newDir.SetY(0);
			}
		}
	}
}