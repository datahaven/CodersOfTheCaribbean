#pragma once // TAG_REMOVE
#include <vector> // TAG_REMOVE
#include "Ship.h" // TAG_REMOVE
#include "PlayerShip.h" // TAG_REMOVE
#include "EnemyShip.h" // TAG_REMOVE
#include "Barrel.h" // TAG_REMOVE

using namespace std;

class GameController
{
public:
	vector<PlayerShip> m_PlayerShips;
	vector<EnemyShip> m_EnemyShips;
	vector<Barrel> m_Barrels;

	int m_TurnCount = 0;
	void StartNextTurn();
	void InputComplete();
	void UpdatePlayerShip(int entityId, int x, int y, int speed, int rotation, int rum);
	void UpdateEnemyShip(int entityId, int x, int y, int speed, int rotation, int rum);
	void AddBarrel(int entityId, int x, int y, int rum);
	void AddMine(int entityId, int x, int y);
	void AddCannonball(int entityId, int x, int y, int firedBy, int turnsToImpact);
	void OutputMoves();
	void GetNearestEnemyToPosition(Vector2 pos, EnemyShip *enemyShip, int *distance);
};

extern GameController GC;