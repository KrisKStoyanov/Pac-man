#include "MovableGameEntity.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, float movementSpeed)
: GameEntity(aPosition), m_movementSpeed(movementSpeed)
{
	m_currentTile = m_nextTile = aPosition;
}

MovableGameEntity::~MovableGameEntity(void)
{
}

