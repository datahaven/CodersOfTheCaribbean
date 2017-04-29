#include "GameController.h" // TAG_REMOVE
#include "HexGrid.h" // TAG_REMOVE
#include <algorithm> // TAG_REMOVE
#include <iostream> // TAG_REMOVE

using namespace std;

GameController GC;

void GameController::StartNextTurn()
{
	m_TurnCount++;
	m_Barrels.clear();
	m_EnemyShips.clear();
}

void GameController::InputComplete()
{
	// This fn is used to remove dead ships so they don't get processed
	m_PlayerShips.erase(
		remove_if(m_PlayerShips.begin(), m_PlayerShips.end(), [&](const PlayerShip ps)-> bool {
															return ps.m_TurnSeenAlive < m_TurnCount;
	}),	m_PlayerShips.end());
}

void GameController::UpdatePlayerShip(int entityId, int x, int y, int speed, int rotation, int rum)
{
	auto it = find_if(GC.m_PlayerShips.begin(), GC.m_PlayerShips.end(), [&entityId](const PlayerShip& ps) {
		return ps.m_Id == entityId;
	});
	PlayerShip e;
	if (it == GC.m_PlayerShips.end())
	{
		GC.m_PlayerShips.push_back(e);
		it = GC.m_PlayerShips.end() - 1;
		it->m_CannonFiredTurn = -1;
		it->m_MinePlacedTurn = -1;
		it->m_CurrentTarget = Vector2(-1, -1);
	}
	it->m_Id = entityId;
	it->m_Position = Vector2(x, y);
	it->m_Rotation = rotation;
	it->m_Speed = speed;
	it->m_Rum = rum;
	it->m_TurnSeenAlive = m_TurnCount;
}

void GameController::UpdateEnemyShip(int entityId, int x, int y, int speed, int rotation, int rum)
{
	// TODO - not sure if I need an update mechanism like the one for players?
	// It'll depend whether or not I want to store any enemy state across turns
	EnemyShip e;
	e.m_Id = entityId;
	e.m_Position = Vector2(x, y);
	e.m_Rotation = rotation;
	e.m_Speed = speed;
	e.m_Rum = rum;
	m_EnemyShips.push_back(e);
}

void GameController::AddBarrel(int entityId, int x, int y, int rum)
{
	Barrel e;
	e.m_Id = entityId;
	e.m_Position = Vector2(x, y);
	e.m_Rum = rum;
	m_Barrels.push_back(e);
}

void GameController::AddMine(int entityId, int x, int y)
{

}

void GameController::AddCannonball(int entityId, int x, int y, int firedBy, int turnsToImpact)
{

}

void GameController::GetNearestEnemyToPosition(Vector2 pos, EnemyShip *enemyShip, int *distance)
{
	*distance = 1000;
	for (const auto &ship : GC.m_EnemyShips)
	{
		int shipDist = HexGrid::Distance(pos, ship.m_Position);
		if (shipDist < *distance)
		{
			*distance = shipDist;
			*enemyShip = ship;
		}
	}
	if (*distance == 1000)
	{
		// Flag to say no enemy found - shouldn't ever happen.
		*distance = -1;
	}
}

void GameController::OutputMoves()
{
	// This should be all I need for a functioning (badly!) bot
	for (auto &ship : GC.m_PlayerShips)
	{
		bool shotFired = false;
		int nearestEnemyDistance = -1;
		EnemyShip nearestEnemy;
		Vector2 myBowPos = ship.GetBowPosition();
		GetNearestEnemyToPosition(myBowPos, &nearestEnemy, &nearestEnemyDistance);
		if (nearestEnemyDistance <= 10 && ship.IsShootAllowedOnTurn(m_TurnCount))
		{
			Vector2 aimPos = nearestEnemy.m_Position;
			bool aimPosFound = false;
			for (int t = 1; t <= nearestEnemyDistance; t++)
			{
				Vector2 enemyPosAtT = nearestEnemy.GetPositionInNTurns(t);
				//cerr << "Id: " << gPlayerShips[i].m_Id << " t: " << t << " ep: " << enemyPosAtT.first << "," << enemyPosAtT.second
				//	<< " range: " << distance(myBowPos, enemyPosAtT)
				//	<< " rtt: " << rangeToTurns(distance(myBowPos, enemyPosAtT)) << endl;
				if (HexGrid::RangeToTurns(HexGrid::Distance(myBowPos, enemyPosAtT)) == t)
				{
					if (HexGrid::IsValidGridPosition(enemyPosAtT))
					{
						aimPos = enemyPosAtT;
						aimPosFound = true;
						break; // So we find earliest hit
					}
				}
			}

			if (aimPosFound)
			{
				cout << "FIRE " << aimPos.x << " "
					<< aimPos.y << endl;
				ship.m_CannonFiredTurn = m_TurnCount;
				shotFired = true;
			}
		}

		if (shotFired == false)
		{
			// Head for nearest barrel
			size_t nearestBarrel = -1;
			int nearestBarrelDistance = 10000;
			for (size_t i = 0; i < m_Barrels.size(); i++)
			{
				int barrelDistance = HexGrid::Distance(ship.m_Position, m_Barrels[i].m_Position);
				if (barrelDistance < nearestBarrelDistance)
				{
					nearestBarrel = i;
					nearestBarrelDistance = barrelDistance;
				}
			}

			if (nearestBarrel != -1)
			{
				cout << "MOVE " << m_Barrels[nearestBarrel].m_Position.x << " " << m_Barrels[nearestBarrel].m_Position.y << endl;
			}
			else
			{
				cout << "WAIT" << endl;
			}
		}
	}
}
