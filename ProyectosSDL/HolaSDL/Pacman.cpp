#include "Pacman.h"
#include "Game.h"


Pacman::Pacman(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC)
	: GameCharacter(posAct, casillaWidth, casillaHeight, gameC, posIniC, dirC, textureC, coorTextureC), newDir(dirC), eating(false), eatingDistance(2000){
 }

Pacman::Pacman(ifstream& file, Game* gameC): GameCharacter(file, gameC){
	int newDirX, newDirY;
	file >> newDirY >> newDirX;
	newDir.SetY(newDirY);
	newDir.SetX(newDirX);

}
void Pacman::render() const {
	
	SDL_Rect destRect = getDestRect();
	//si est� en modo caza tiene distinto sprite
	if(!eating)texture->renderFrame(destRect, coorTexture.GetY(), coorTexture.GetX());
	else {texture->renderFrame(destRect, coorTexture.GetY(), coorTexture.GetX() + 1);}
}


void Pacman::comerAlimento() {
	//Pueden ser o vitaminas o comida
	if (game->contenidoCelda(pos.GetY(), pos.GetX()) == Food) {
		//si es comida la elmina del mapa y la resta del contador de comida restante, cuando este contador llegue a 0 el juego se acaba
		game->Comida();
		game->EmptyCell(pos.GetY(), pos.GetX());
	}
	else if (game->contenidoCelda(pos.GetY(), pos.GetX()) == Vitamins) {
		//si es vitamina se pone en modo caza y se elimina esa vitamina del mapa, no hace falta recogerlas todas para ganar
		eating = true;
		eatingDistance = 600;
		game->EmptyCell(pos.GetY(), pos.GetX());
	}
}

void Pacman::update() {
	
	//Se obtiene el rect�ngulo destino gracias al m�todo del GameObject, devuelve la posici�n ya en pixeles y no en coordenadas de la casilla del mapa
	SDL_Rect destRect = getDestRect();

	Point2D newPos;
	//primero comprueba si se puede mover a la direcci�n de la �ltima tecla pulsada, si no, sigue en la direcci�n en la que estaba yendo
	if (game->tryMove(destRect, newDir, newPos)){
		dir = newDir;
		pos = newPos;
	}
	else if(game->tryMove(destRect, dir, newPos)){
		pos = newPos;
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

//comprueba los eventos, el pacman no se encarga de controlar la pulsaci�nde la "s" para guardar la partida, eso lo hace Game
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

void Pacman::saveToFile(std::ofstream& file) {
	file << 0 << " ";//este cero es para saber que es el pacman
	GameCharacter::saveToFile(file);
	file << newDir.GetY() << " " << newDir.GetX() << "\n";
}