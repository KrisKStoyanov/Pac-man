#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2f.h"

class MovableGameEntity : public GameEntity
{
public:
	MovableGameEntity(const Vector2f& aPosition, const char* anImage);
	~MovableGameEntity(void);

	inline void SetCurrentTile(const Vector2f& tile) { m_currentTile = tile; }
	inline void SetNextTile(const Vector2f& tile) { m_nextTile = tile; }

	inline Vector2f GetCurrentTile() { return m_currentTile; }
	inline Vector2f GetNextTile() { return m_nextTile; }

	bool IsAtDestination();

protected:

	Vector2f m_currentTile;
	Vector2f m_nextTile;

};

#endif // MOVABLEGAMEENTITY_H