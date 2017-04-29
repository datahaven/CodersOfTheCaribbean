#include "Vector2.h" // TAG_REMOVE
#include "HexGrid.h" // TAG_REMOVE
#include <cstdlib> // TAG_REMOVE

Vector2 HexGrid::Neighbour(Vector2 position, int orientation)
{
	int DIRECTIONS_EVEN[][2] = { { 1, 0 },{ 0, -1 },{ -1, -1 },{ -1, 0 },{ -1, 1 },{ 0, 1 } };
	int DIRECTIONS_ODD[][2] = { { 1, 0 },{ 1, -1 },{ 0, -1 },{ -1, 0 },{ 0, 1 },{ 1, 1 } };
	int newY, newX;
	if (position.y % 2 == 1) {
		newY = position.y + DIRECTIONS_ODD[orientation][1];
		newX = position.x + DIRECTIONS_ODD[orientation][0];
	}
	else {
		newY = position.y + DIRECTIONS_EVEN[orientation][1];
		newX = position.x + DIRECTIONS_EVEN[orientation][0];
	}

	return Vector2(newX, newY);
}

bool HexGrid::IsValidGridPosition(Vector2 pos)
{
	return pos.x >= 0 && pos.y >= 0 && pos.x < HexGrid::MAP_WIDTH && pos.y <= HexGrid::MAP_HEIGHT;
}

void HexGrid::Offset_to_cube(Vector2 off, int *cx, int *cy, int *cz)
{
	*cx = off.x - (off.y - (off.y & 1)) / 2;
	*cz = off.y;
	*cy = -(*cx) - *cz;
}

int HexGrid::Distance(Vector2 a, Vector2 b)
{
	int acx, acy, acz;
	HexGrid::Offset_to_cube(a, &acx, &acy, &acz);
	int bcx, bcy, bcz;
	HexGrid::Offset_to_cube(b, &bcx, &bcy, &bcz);
	return (abs(acx - bcx) + abs(acy - bcy) + abs(acz - bcz)) / 2;
}

int HexGrid::RangeToTurns(int range)
{
	int RANGE[] = { 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4 };
	return RANGE[range] + 1;
}