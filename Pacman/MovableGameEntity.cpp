#include "MovableGameEntity.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, const char* anImage)
: GameEntity(aPosition, anImage)
{
	m_currentTile = m_nextTile = Vector2f(myPosition.myX / 22, myPosition.myY / 22);
}

MovableGameEntity::~MovableGameEntity(void)
{
}

bool MovableGameEntity::IsAtDestination()
{
	if (m_currentTile.myX == m_nextTile.myX && m_currentTile.myY == m_nextTile.myY) 
	{
		return true;
	}

	return false;
}
