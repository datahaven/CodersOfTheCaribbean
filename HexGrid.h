#pragma once
#include "Vector2.h" // TAG_REMOVE
class HexGrid
{
public:
	static const int MAP_WIDTH = 23;
	static const int MAP_HEIGHT = 21;
	static Vector2 Neighbour(Vector2 position, int orientation);
	static bool IsValidGridPosition(Vector2 pos);
	static void Offset_to_cube(Vector2 off, int *cx, int *cy, int *cz);
	static int Distance(Vector2 a, Vector2 b);
	static int RangeToTurns(int range);
};
