#include "Ship.h" // TAG_REMOVE
#include "HexGrid.h" // TAG_REMOVE

Vector2 Ship::GetBowPosition() const
{
	return HexGrid::Neighbour(m_Position, m_Rotation);
}

Vector2 Ship::GetSternPosition() const
{
	return HexGrid::Neighbour(m_Position, (m_Rotation + 3) % 6);
}

bool Ship::IsShootAllowedOnTurn(int t) const
{
	return m_CannonFiredTurn < t - 1;
}

#if 0
void Ship::GetPositionAfterMove(ActionType a, Vector2 inPos, int inSpeed, int inRotation,
	Vector2 &outPos, int &outSpeed, int &outRotation)
{
	// This will allow for setting speed to 0 if we run off map
	// But doesn't check for ship/ship collisions as they might move out of the way later
	outPos = inPos;
	outRotation = inRotation;
	outSpeed = inSpeed;

	if (a == ActionType::SLOWER && outSpeed > 0)
	{
		--outSpeed;
	}
	else if (a == ActionType::FASTER && outSpeed < 2)
	{
		++outSpeed;
	}
	else if (a == ActionType::PORT)
	{
		outRotation = (outRotation + 1) % 6;
	}
	else if (a == ActionType::STARBOARD)
	{
		outRotation = (outRotation + 5) % 6;
	}

	for (int s = 1; s <= outSpeed; ++s)
	{
		Vector2 oldPos = outPos;
		outPos = HexGrid::Neighbour(outPos, inRotation);
		if (isValidPosition(outPos) == false)
		{
			outPos = oldPos;
			outSpeed = 0;
		}
	}
}

void Ship::PopulatePossibleActions(vector<ShipAction> &outActions, Vector2 inPos, int inSpeed, int inRotation)
{
	// This tries out all possible movement actions.actions
	// Note that FIRE and MINE are equivalent to a WAIT in this case
	outActions.clear();

	ShipAction action;

	action.m_Type = ActionType::WAIT;
	Ship::GetPositionAfterMove(action.m_Type, inPos, inSpeed, inRotation, action.m_NewPosition, action.m_NewSpeed, action.m_NewRotation);
	outActions.push_back(action);

	if (inSpeed > 0)
	{
		action.m_Type = ActionType::SLOWER;
		GetPositionAfterMove(action.m_Type, inPos, inSpeed, inRotation, action.m_NewPosition, action.m_NewSpeed, action.m_NewRotation);
		outActions.push_back(action);
	}

	if (inSpeed < 2)
	{
		action.m_Type = ActionType::FASTER;
		GetPositionAfterMove(action.m_Type, inPos, inSpeed, inRotation, action.m_NewPosition, action.m_NewSpeed, action.m_NewRotation);
		outActions.push_back(action);
	}

	action.m_Type = ActionType::PORT;
	GetPositionAfterMove(action.m_Type, inPos, inSpeed, inRotation, action.m_NewPosition, action.m_NewSpeed, action.m_NewRotation);
	outActions.push_back(action);

	action.m_Type = ActionType::STARBOARD;
	GetPositionAfterMove(action.m_Type, inPos, inSpeed, inRotation, action.m_NewPosition, action.m_NewSpeed, action.m_NewRotation);
	outActions.push_back(action);
}

void Ship::RatePossibleActions()
{
	for (auto it = m_PossibleActions.begin(); it != m_PossibleActions.end(); it++)
	{
		RatePossibleAction(*it);
	}

	sort(m_PossibleActions.begin(), m_PossibleActions.end(), [&](const ShipAction &a, const ShipAction &b) -> bool {
		return a.m_EvaluationScore > b.m_EvaluationScore;
	});
}

void Ship::RatePossibleAction(ShipAction &a)
{
	a.m_EvaluationScore = 0;

	// Need to prioritise actions so WAIT (aka FIRE/MINE) scores higher than moving
	if (a.m_Type == ActionType::WAIT)
	{
		a.m_EvaluationScore += 1;
	}

	a.m_DamageSustained = 1; // Damage per turn
	a.m_DamageDealt = 0;
	a.m_DamageHealed = 0;
	Vector2 shipBowPos = GetBowPosition();
	Vector2 shipSternPos = GetSternPosition();
	for (const auto &cb : gCannonballs)
	{
		if (cb.m_TurnsToImpact == 1)
		{
			if (shipBowPos == cb.m_Position || shipSternPos == cb.m_Position)
			{
				a.m_DamageSustained += 25;
			}
			else if (m_Position == cb.m_Position)
			{
				a.m_DamageSustained += 50;
			}
		}
	}

	for (const auto &m : gMines)
	{
		if (shipBowPos == m.m_Position || shipSternPos == m.m_Position || m_Position == m.m_Position)
		{
			a.m_DamageSustained += 25;
		}
	}

	// TODO - I should track Damage from exploded mines
	// s/b straightforward to do - will have to remove mines hit by cannonballs and create a gDamage vector

	for (const auto &b : gBarrels)
	{
		if (shipBowPos == b.m_Position || shipSternPos == b.m_Position || m_Position == b.m_Position)
		{
			a.m_DamageHealed += b.m_Rum;
		}
	}

	// TODO - should cap this at ship min/max health, with min getting a large -ve score
	a.m_EvaluationScore = a.m_DamageHealed - a.m_DamageSustained;
}
#endif
Vector2 Ship::GetPositionInNTurns(int N)
{
	// Assuming ship keeps moving at same speed and rotation
	Vector2 nextPos = m_Position;
	for (int n = 1; n <= N; n++)
	{
		for (int s = 0; s<m_Speed; ++s)
		{
			nextPos = HexGrid::Neighbour(nextPos, m_Rotation);
		}
	}
	return nextPos;
}