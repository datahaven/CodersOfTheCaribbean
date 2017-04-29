#pragma once // TAG_REMOVE
#include <vector> // TAG_REMOVE
#include "Entity.h" // TAG_REMOVE
#include "Vector2.h" // TAG_REMOVE
class Ship : public Entity
{
public:
	int m_Rotation;
	int m_Speed;
	int m_Rum;
	int m_CannonFiredTurn = -1;
	int m_MinePlacedTurn = -1;
	//std::vector<ShipAction> m_PossibleActions;

	Vector2 GetBowPosition() const;
	Vector2 GetSternPosition() const;

	// This assumes ship doesn't change speed or rotation
	Vector2 GetPositionInNTurns(int n);

	bool IsShootAllowedOnTurn(int t) const;
	//static void GetPositionAfterMove(ActionType a, Vector2 inPos, int inSpeed, int inRotation,
	//	Vector2 &outPos, int &outSpeed, int &outRotation);

	// TODO - a separate fn for ship prediction

	//static void PopulatePossibleActions(std::vector<ShipAction> &outActions, Vector2 currentPos, int currentSpeed, int currentRotation);
	//void RatePossibleActions();
	//void RatePossibleAction(ShipAction &a);
};
