#include "Vector2D.h"


Vector2D  Vector2D::operator+(const Vector2D& otroVector) const{
	Vector2D suma;
	suma.y = x + otroVector.y;
	suma.x = x + otroVector.x;
	return suma;
}

Vector2D  Vector2D::operator-(const Vector2D& otroVector) const {
	Vector2D resta;
	resta.y = x - otroVector.y;
	resta.x = x - otroVector.x;
	return resta;
}
//Devuelve un vector inverso al qu está(no se pueden diagonlaes ya que no hay movimiento diagonal)
void Vector2D::Invierte() {
	x *= -1;
	y *= -1;
}

//Devuelve un vecctor perpendicular al que está
void Vector2D::Perpendicular() {
	int aux;
	aux = x;
	x = y;
	y = aux;
}

int  Vector2D::operator*(const Vector2D& otroVector) const {
	
	return otroVector.y * y + otroVector.x * x;

}

Vector2D  Vector2D::operator*(const int& num) const {
	Vector2D mul;
	mul.y = y * num;
	mul.x = x * num;
	return mul;
}


bool  Vector2D::operator==(const Vector2D& otroVector) const {
	
	return ((y == otroVector.y) && (x == otroVector.x));
}


	
