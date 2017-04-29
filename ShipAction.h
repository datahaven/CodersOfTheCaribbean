#pragma once // TAG_REMOVE
#include "Vector2.h" // TAG_REMOVE

enum class ActionType : char { WAIT = 0, SLOWER, FASTER, PORT, STARBOARD, FIRE, MINE /*, MOVE*/ };

class ShipAction
{
public:
	ActionType m_Type;
	// Two positions/rotations because at speed two, the ship will hit two different
	// positions/rotations.
	// 0 for final pos, 1 for intermediate pos. Might change this to something less easy to muddle up!
	Vector2 m_NewPosition[2];
	int m_NewRotation[2];
	int m_NewSpeed;
	//int m_DamageSustained;
	//int m_DamageDealt;
	//int m_DamageHealed;
	//int m_EvaluationScore;
};
