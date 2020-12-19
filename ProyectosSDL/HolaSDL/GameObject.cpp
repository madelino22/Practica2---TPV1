#include "GameObject.h"

SDL_Rect GameObject::getDestRect() const{

	//crea el rectángulo en el que el objeto se va a pintar y lo devuelve
	SDL_Rect rectMap;
	rectMap.x = pos.GetX();
	rectMap.y = pos.GetY();
	rectMap.w = width;
	rectMap.h = height;

	return rectMap;
}