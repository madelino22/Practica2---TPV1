#include "GameObject.h"

SDL_Rect GameObject::getDestRect() {

	SDL_Rect rectMap;
	//esto sería el margen si hibiera desde la izquierda
	rectMap.x = pos.GetX() * width;
	rectMap.y = pos.GetY() * height;
	rectMap.w = width;
	rectMap.h = height;

	return rectMap;
}