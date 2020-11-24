#pragma once


class Vector2D
{
private:
	int x;
	int y;
	

public:
	Vector2D() { y = 0; x = 0;};
	Vector2D(int a, int b) { x = a; y = b; };

	int GetY() const { return y; };
	int GetX() const { return x; };

	void SetY(int b) { y = b; }
	void SetX(int a) { x = a; }

	void Invierte();
	void Perpendicular();

	Vector2D operator+(const Vector2D& otroVector) const;
	Vector2D operator-(const Vector2D& otroVector) const;
	int  operator*(const Vector2D& otroVector) const;
	Vector2D operator*(const int& num) const;
	bool operator==(const Vector2D& otroVector) const;
}; using Point2D = Vector2D;

