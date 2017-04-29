#pragma once
#include "Entity.h" // TAG_REMOVE
class Cannonball : public Entity
{
public:
	int m_FiredBy;
	int m_TurnsToImpact;
};

