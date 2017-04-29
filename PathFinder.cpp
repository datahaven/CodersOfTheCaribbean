#include "PathFinder.h" // TAG_REMOVE
#include "ShipAction.h" // TAG_REMOVE
#include "Ship.h" // TAG_REMOVE
#include "HexGrid.h" // TAG_REMOVE
#include <cstring>
#include <queue>
using namespace std;

void PathFinder::FindAllPaths(Vector2 pos, int speed, int rot, bool stopOnFirstBarrel)
{
	// Starting from pos/speed/rot, use Breadth First Search to find the
	// earliest time we can get to any position (ie at pos/speed/rot) on the board

	// With help from http://www.geeksforgeeks.org/breadth-first-traversal-for-a-graph/

	// ShipPosition == 0 means pos is not valid
	// Use this to mark all states as not visited
	memset(m_AllPaths, '\0', sizeof(m_AllPaths));
	m_FirstBarrelPos = Vector2(-1, -1);
	queue<ShipPosition> posQueue;

	// Mark current node as visited ...
	ShipPosition startShipPos(pos, speed, rot);
	m_AllPaths[startShipPos.m_Bitfield] = startShipPos; // point to self to mark start pos
														// and enqueue it
	posQueue.push(startShipPos);

	while (posQueue.empty() == false)
	{
		ShipPosition sp = posQueue.front();
		posQueue.pop();

		// Find all possible moves from this one
		vector<ShipAction> possibleActions;
		// TODO - PopulatePossibleActions
		//Ship::PopulatePossibleActions(possibleActions, sp.GetPosition(), sp.GetSpeed(), sp.GetRotation());
		for (const auto & pa : possibleActions)
		{
			// If this action didn't move the ship, then don't allow it
			// This happens if we try to accelerate into the edge of the map
			if (pa.m_NewPosition[0] == sp.GetPosition() && pa.m_NewRotation[0] == sp.GetRotation() && pa.m_NewSpeed == sp.GetSpeed())
			{
				continue;
			}

			// If this position takes us into contact with a mine, then don't allow it
			Vector2 newBowPosition = HexGrid::Neighbour(pa.m_NewPosition[0], pa.m_NewRotation[0]);
			Vector2 newSternPosition = HexGrid::Neighbour(pa.m_NewPosition[0], (pa.m_NewRotation[0] + 3) % 6);
			//if (gMineMap.IsCellOccupied(pa.m_NewPosition.first, pa.m_NewPosition.second) ||
			//	gMineMap.IsCellOccupied(newBowPosition.first, newBowPosition.second) ||
			//	gMineMap.IsCellOccupied(newSternPosition.first, newSternPosition.second))
			//{
			//	continue;
			//}

			//if (gShipMap.IsCellOccupied(pa.m_NewPosition.first, pa.m_NewPosition.second) ||
			//	gShipMap.IsCellOccupied(newBowPosition.first, newBowPosition.second) ||
			//	gShipMap.IsCellOccupied(newSternPosition.first, newSternPosition.second))
			//{
			//	continue;
			//}

			// TODO - disallow collisions with other ships.
			// NB Will need to change ShipMap so we don't check collision with ourself

			// TODO - if this position would take cannon damage, then avoid. Need to know turn number for that.

			// If this position collects a barrel we may want to note that? Possibly terminate early?
			//if (m_FirstBarrelPos.first == -1 &&
			//	(gBarrelMap.IsCellOccupied(pa.m_NewPosition.first, pa.m_NewPosition.second)))//||
				//gBarrelMap.IsCellOccupied(newBowPosition.first, newBowPosition.second) ||
				//gBarrelMap.IsCellOccupied(newSternPosition.first, newSternPosition.second)))
			//{
			//	m_FirstBarrelPos = pa.m_NewPosition;
			//	m_FirstBarrelShipPosition = ShipPosition(pa.m_NewPosition, pa.m_NewSpeed, pa.m_NewRotation);
			//	if (stopOnFirstBarrel)
			//	{
			//		goto end_loop;
			//	}
			//}

			// If position hasn't been reached then mark it as reached and enqueue it
			ShipPosition newPos = ShipPosition(pa.m_NewPosition[0], pa.m_NewSpeed, pa.m_NewRotation[0]);
			if (m_AllPaths[newPos.m_Bitfield].isValidPosition() == false)
			{
				m_AllPaths[newPos.m_Bitfield] = sp;
				posQueue.push(newPos);
			}
		}
	}
end_loop:
	;
}

vector<ActionType> PathFinder::GetMovesToPosition(Vector2 destPos)
{
	// Assuming FindAllPaths has been called, what is the best path
	// to destPos?

	// There are 3 speeds * 6 rotations == 18 different states for each
	// position. Do I need to check them all to see which one is quickest?
	// Also need to check I'm not overshooting into danger
	// Could store the depth in m_AllPaths, then it's quick to check

	// Assuming we want to be stationary when we get there - not a great assumption!
	//GetMovesToShipPosition(destPos, 0, 0);

	// Try all states
	int bestMoves = 10000;
	int bestFinishR = 0;
	int bestFinishS = 0;
	vector<ActionType> bestActions;
	for (int s = 2; s >= 0; --s)
	{
		for (int r = 0; r < 6; ++r)
		{
			vector<ActionType> actions = GetMovesToShipPosition(destPos, s, r);
			if (actions.size() < static_cast<size_t>(bestMoves))
			{
				bestMoves = actions.size();
				bestFinishR = r;
				bestFinishS = s;
				bestActions = actions;
				//cout << "New Best!" << endl;
			}
			// TODO - I could track joint equal best moves, then
			// keep a list of options for the starting move.
			// eg to see if I prefer WAIT, then TURN over TURN, WAIT, say for shooting
			// For now, the above loop order should mean actions with a faster finish
			// speed prioritise higher
		}
	}
	reverse(bestActions.begin(), bestActions.end());
	return bestActions;
}

vector<ActionType> PathFinder::GetMovesToShipPosition(Vector2 destPos, int speed, int rotation)
{
	// Assuming FindAllPaths has been called, what is the best path
	// to destPos?
	vector<ActionType> actions;

	ShipPosition destShipPos(destPos, speed, rotation);
	ShipPosition nextShipPos = destShipPos;
	int count = 0;
	while (nextShipPos.m_Bitfield != m_AllPaths[nextShipPos.m_Bitfield].m_Bitfield)
	{
		++count;

		ShipPosition fromPos = m_AllPaths[nextShipPos.m_Bitfield];
		ShipPosition toPos = nextShipPos;
		ActionType a = GetMoveToCell(m_AllPaths[nextShipPos.m_Bitfield], nextShipPos);
		actions.push_back(a);
		//cout << "From: " << fromPos.GetPosition().first << "," << fromPos.GetPosition().second 
		//		<< " s:" << fromPos.GetSpeed() << " r:" << fromPos.GetRotation() << " - ";
		//cout << "To: " << toPos.GetPosition().first << "," << toPos.GetPosition().second
		//	<< " s:" << toPos.GetSpeed() << " r:" << toPos.GetRotation() << " : ";
		//cout << ActionTypeToString(GetMoveToCell(m_AllPaths[nextShipPos.m_Bitfield], nextShipPos)) << endl;

		nextShipPos = m_AllPaths[nextShipPos.m_Bitfield];
	}
	//cout << count << " moves to position" << endl;
	return actions;
}

ActionType PathFinder::GetMoveToCell(ShipPosition fromPos, ShipPosition toPos)
{
	if (fromPos.GetSpeed() > toPos.GetSpeed())
	{
		return ActionType::SLOWER;
	}
	else if (fromPos.GetSpeed() < toPos.GetSpeed())
	{
		return ActionType::FASTER;
	}
	else if (fromPos.GetRotation() != toPos.GetRotation())
	{
		if ((fromPos.GetRotation() + 1) % 6 == toPos.GetRotation())
		{
			return ActionType::PORT;
		}
		else if ((fromPos.GetRotation() + 5) % 6 == toPos.GetRotation())
		{
			return ActionType::STARBOARD;
		}
		//cout << "ERROR!" << endl;
	}
	return ActionType::WAIT;
}

