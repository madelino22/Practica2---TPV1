#include "Ghost.h"
#include "Game.h"



Ghost::Ghost(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC, int colorC):
	GameCharacter(posAct, casillaWidth, casillaHeight, gameC, posIniC, dirC, textureC, coorTextureC), color(colorC){

}


Ghost::Ghost(ifstream& file, Game* gameC): GameCharacter(file, gameC) {
	file >> color;
}

bool Ghost::pacmanCollison() {
	return SDL_HasIntersection(&getDestRect(), &game->GetPacmanRect());
}
bool Ghost::comer(Point2D posPacMan) {
	bool comido = false;
	if (pacmanCollison()) {
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
	pos = posIni;
}
void Ghost::render() const {


	//creacion e inicializacion del rectángulo destino
	SDL_Rect destRect = getDestRect();
	texture->renderFrame(destRect, 0, color*2);
	
}

//Aqui hay que hacer que decida de manera aleatoria donde ir
void Ghost::update() {
	
	
	Vector2D posibles[3];
	Point2D casillasPosibles[3];
	Vector2D dirPos = dir;
	int tam = 0;

	SDL_Rect rect = getDestRect();;
	


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
		pos = destino;
	}
	
	else {
		//selecciona al azar de entre una de las posibles
		int nuevaDir = rand() % tam;
		dir = posibles[nuevaDir];
		pos = casillasPosibles[nuevaDir];
	}
	
	
	comer(game->getPacManPosAct());
	//Come antes y depués del movimiento para evitar que se crucen sin interactuar

	
}



void Ghost::saveToFile(std::ofstream& file) {
	file << 1 << " ";//este cero es para saber que es el pacman
	GameCharacter::saveToFile(file);
	file << color << "\n";
 }