#include "Ghost.h"
#include "Game.h"



Ghost::Ghost(Point2D posAct, int casillaWidth, int casillaHeight, Game* gameC, Point2D posIniC, Vector2D dirC, Texture* textureC, Point2D coorTextureC):
	GameCharacter(posAct, casillaWidth, casillaHeight, gameC, posIniC, dirC, textureC, coorTextureC){
	cooldown = 200;
	
}


Ghost::Ghost(ifstream& file, Game* gameC): GameCharacter(file, gameC) {
	//al contrario que el pacman, ghost no tiene variables importantes a parte de las que guarda gameCharacter, por lo que no se guarda ni hace lectura de nada más
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

	SDL_Rect destRect = getDestRect();
	//ahora coortexture guarda la coordenada de latextura, por lo que no hace falta guardar una variable para el colo del fantasma
	texture->renderFrame(destRect, 0, coorTexture.GetX());
	
}

//Aqui hay que hacer que decida de manera aleatoria donde ir
void Ghost::update() {
	cooldown--;
	SDL_Rect rect = getDestRect();
	if (capMov < 0)
	{
		capMov = 7;
		Vector2D posibles[3]; Point2D casillasPosibles[3];
		Vector2D dirPos = dir;
		int tam = 0;
		for (int x = 0; x < 3; x++) {
			Point2D casillaDestino;
			//primero checkeo si en la direcion que está yendo se puede,
			if (game->tryMove(rect, dirPos, casillaDestino)) {
				posibles[tam] = dirPos;
				casillasPosibles[tam] = casillaDestino;
				tam++;
			}
			//luego una de las de al lado(perpendicular) y al final la inversa a la última(la que era perpendicular)
			//así siempre se chequean las direcciones que no es marcha atrás, si ninguna es posible, no queda otra que ir hacia atrás
			if (x == 0) dirPos.Perpendicular();
			else if (x == 1) dirPos.Invierte();
		}
		//`ya no hace falta que coma depues y antes del moviemiento ya que checkea el solape y no la posición
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
		
	}
	else {
		Point2D newPos;
		if (game->tryMove(rect, dir, newPos)){
			pos = newPos;
		}
	}
	capMov--;
}



void Ghost::saveToFile(std::ofstream& file) {
	file << 1 << " ";//este uno es para saber que es un fantasma
	GameCharacter::saveToFile(file);
	//al acabar de guardar todo lo necesario del ghost salta línea
	file << "\n";
 }