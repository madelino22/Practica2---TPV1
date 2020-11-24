
#include "Texture.h"
#include <iostream>

using namespace std;

void Texture::Free() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
	w = h = 0;
}


//Se carga una textura desde un archivo
void Texture::load(string filename, uint nRows, uint nCols) {
	SDL_Surface* tempSurface = IMG_Load(filename.c_str());
	if (tempSurface == nullptr) throw "Error cargando surface desde " + filename;
	Free();
	texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (texture == nullptr) throw "Error cargando texture desde " + filename;
	
	numRows = nRows;
	numCols = nCols;

	//tamaño de la textura
	w = tempSurface->w;
	h = tempSurface->h;

	//tamaño de cada celda
	fw = w / numCols;
	fh = h / numRows;
	SDL_FreeSurface(tempSurface);
}


//renderiza la imagen entera
void Texture::render(const SDL_Rect& destRect, SDL_RendererFlip flip) const {
	// Se establece el rectangulo fuente con el tamaño de la textura entera
	SDL_Rect srcRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = w; srcRect.h = h;
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, 0, flip);
}


//renderiza una parte de una spritesheet indicadndo la posición con row y col y la rotación de la imagen resultante con angle
void Texture::renderFrame(const SDL_Rect& destRect, int row, int col, int angle, SDL_RendererFlip flip) const {
	//Se establece el rectangulo fuente, uqe cogerá el rectángulo dado en los parámetros
	SDL_Rect srcRect;
	srcRect.x = fw * col;
	srcRect.y = fh * row;
	srcRect.w = fw;
	srcRect.h = fh;
	SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, 0, flip);
}