#include "GameEntity.h"

GameEntity::GameEntity(const Vector2f& aPosition) : 
	myPosition(aPosition), 
	myIdMarkedForDeleteFlag(false),
	m_drawOffsetX(220.0f), m_drawOffsetY(60.0f)
{
	m_drawPos = Vector2f(
		aPosition.myX + m_drawOffsetX, 
		aPosition.myY + m_drawOffsetY);
}

GameEntity::~GameEntity(void)
{
}
