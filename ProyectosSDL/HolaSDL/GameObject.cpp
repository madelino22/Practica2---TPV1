#include "GameObject.h"

SDL_Rect GameObject::getDestRect() const{

	SDL_Rect rectMap;
	//esto sería el margen si hibiera desde la izquierda
	rectMap.x = pos.GetX();
	rectMap.y = pos.GetY();
	rectMap.w = width;
	rectMap.h = height;

	return rectMap;
}