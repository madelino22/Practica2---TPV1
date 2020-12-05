#include "GameMap.h"
#include "Game.h"

GameMap::GameMap(int nFils, int nCols, Game* g, Texture* textWall, Texture* textVit, Texture* textFood) {
	fils = nFils;
	cols = nCols;
	celdasMapa = new MapCell * [fils];
	for (int y = 0; y < fils; y++) {
		celdasMapa[y] = new MapCell[cols];
	}
	game = g;
	textureWall = textWall;
	textureVit = textVit;
	textureFood = textFood;

	//Al construir creear el mapa vacío
	for (int y = 0; y < fils; y++) {
		for (int x = 0; x < cols; x++) {
			celdasMapa[y][x] = Empty;
		}
	}

	casillaH = WIN_HEIGHT / fils;
	casillaW = WIN_WIDTH / cols;

}
GameMap::~GameMap() {

	//no se necesita borrar las texturas ya que son punteros que luego los borrará game
	//El primer bucle destruye cada array que sería cada fila

	//Al acabr el pr
	
    for (int x = 0; x < fils; x++) {
		delete[] celdasMapa[x];
    }
	//esto borra al columna
	delete[] celdasMapa;
	
}


SDL_Rect GameMap::getDestRect() {
	SDL_Rect rectMap;
	//esto sería el margen si hibiera desde la izquierda
	rectMap.x = MARGENX;
	rectMap.y = MARGENY;
	rectMap.w = cols * casillaW;
	rectMap.h = fils * casillaH;

	return rectMap;
}
void GameMap::render() const {

	//recorre todas las casillas del mapa pintandola si son muos comida o vitaminas,
	//de la renderización del pacman y ffantasmas se encargan ellos mismos
	for (int x = 0; x < fils; x++) {
		for (int y = 0; y < cols; y++) {
			MapCell tipo = celdasMapa[x][y];

			SDL_Rect destRect;
			destRect.x = y * casillaW;
			destRect.y = x * casillaH;
			destRect.h = casillaH;
			destRect.w = casillaW;

			if (tipo == Wall) {
				textureWall->renderFrame(destRect, 0, 0);
			}
			else if (tipo == Food) {
				textureFood->renderFrame(destRect, 0, 0);
			}
			else if (tipo == Vitamins) {
				textureVit->renderFrame(destRect, 0, 0);
			}
		}
	}
}

bool GameMap::IntersectsWall(SDL_Rect rect) {
	Point2D topLeft =  game->SDLPointToMapCoords(Point2D(rect.y, rect.x));
	Point2D botRight = game->SDLPointToMapCoords(Point2D(rect.y + casillaH-1, rect.x + casillaW-1));

	for (int y = topLeft.GetY(); y <= botRight.GetY(); y++) {
		for (int x = topLeft.GetX(); x <= botRight.GetX(); x++) {
			if (celdasMapa[y][x] == Wall) return true;
		}
	}
	return false;
}
