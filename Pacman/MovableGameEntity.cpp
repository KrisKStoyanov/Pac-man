#include "MovableGameEntity.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, float movementSpeed, int tileSize)
: GameEntity(aPosition), m_movementSpeed(movementSpeed)
{
	m_currentTile = m_nextTile = Vector2f(myPosition.myX / (float)tileSize, myPosition.myY / (float)tileSize);
}

MovableGameEntity::~MovableGameEntity(void)
{
}

bool MovableGameEntity::IsAtDestination()
{
	if (m_currentTile == m_nextTile) 
	{
		return true;
	}
	return false;
}
