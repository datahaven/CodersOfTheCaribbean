#pragma once // TAG_REMOVE
#include "Vector2.h" // TAG_REMOVE
#include "ShipAction.h" // TAG_REMOVE
#include "ShipPosition.h" // TAG_REMOVE
#include <vector> // TAG_REMOVE

class PathFinder
{
private:
	static const int MAX_PATHS = 32768; // 2^15
	ShipPosition m_AllPaths[MAX_PATHS];
	Vector2 m_FirstBarrelPos;
	ShipPosition m_FirstBarrelShipPosition;
public:
	void FindAllPaths(Vector2 pos, int speed, int rot, bool stopOnFirstBarrel = false);
	std::vector<ActionType> GetMovesToPosition(Vector2 destPos);
	std::vector<ActionType> GetMovesToShipPosition(Vector2 destPos, int speed, int rotation);
	static ActionType GetMoveToCell(ShipPosition fromPos, ShipPosition toPos);
};
