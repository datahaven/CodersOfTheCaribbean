#pragma once
struct Vector2
{
	// Default to an Invalid position
	Vector2() : x(-1), y(-1) {};
	Vector2(int ix, int iy) : x(ix), y(iy) {};
	bool operator==(const Vector2 &a) const { return this->x == a.x && this->y == a.y; }
	int x;
	int y;
};
