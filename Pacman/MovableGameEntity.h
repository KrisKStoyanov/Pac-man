#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2f.h"

class MovableGameEntity : public GameEntity
{
public:
	MovableGameEntity(const Vector2f& aPosition, float movementSpeed);
	~MovableGameEntity(void);

	inline void SetCurrentTile(const Vector2f& tile) { m_currentTile = tile; }
	inline void SetNextTile(const Vector2f& tile) { m_nextTile = tile; }

	inline Vector2f GetCurrentTile() { return m_currentTile; }
	inline Vector2f GetNextTile() { return m_nextTile; }

	inline float GetMovementSpeed() { return m_movementSpeed; }

	inline Vector2f GetDirection() { return m_direction; }
	inline void SetDirection(const Vector2f& direction) { m_direction = direction; }

	bool IsAtDestination();

protected:

	float m_movementSpeed;
	Vector2f m_currentTile;
	Vector2f m_nextTile;
	Vector2f m_direction;
};

#endif // MOVABLEGAMEENTITY_H