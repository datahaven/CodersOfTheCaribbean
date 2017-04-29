#pragma once // TAG_REMOVE
#include "Vector2.h" // TAG_REMOVE

class ShipPosition
{
public:
	// x ==> bits 0-4
	// y ==> bits 5-9
	// s ==> bits 10-11, 0 means invalid, 1,2,3 means speed 0,1,2. So we can clear pos by setting m_Bitfield to zero
	// r ==> bits 12-14
	short unsigned int m_Bitfield;

	inline ShipPosition()
	{
		ResetPosition();
	}
	inline ShipPosition(Vector2 pos, int speed, int rotation)
	{
		ResetPosition();
		SetPosition(pos);
		SetSpeed(speed);
		SetRotation(rotation);
	}
	inline void ResetPosition()
	{
		m_Bitfield = 0;
	}
	inline bool isValidPosition() const
	{
		return ((m_Bitfield & 0xc00) >> 10) != 0;
	}
	inline Vector2 GetPosition() const
	{
		return Vector2(static_cast<int>(m_Bitfield & 0x1F), static_cast<int>((m_Bitfield & 0x3E0) >> 5));
	}
	inline void SetPosition(Vector2 pos)
	{
		m_Bitfield |= pos.x | (pos.y << 5);
	}
	inline int GetSpeed() const
	{
		return ((m_Bitfield & 0xc00) >> 10) - 1;
	}
	inline void SetSpeed(int speed)
	{
		m_Bitfield |= (speed + 1) << 10;
	}
	inline int GetRotation() const
	{
		return (m_Bitfield & 0x7000) >> 12;
	}
	inline void SetRotation(int rotation)
	{
		m_Bitfield |= (rotation << 12);
	}
};
