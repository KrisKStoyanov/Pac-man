#include "GameEntity.h"
#include "Core.h"

GameEntity::GameEntity(const Vector2f& aPosition, const char* anImage)
:myPosition(aPosition)
,myImage(anImage)
,myIdMarkedForDeleteFlag(false),
m_drawOffsetX(220.0f), m_drawOffsetY(60.0f)
{
	m_drawPos = Vector2f(
		aPosition.myX + m_drawOffsetX, 
		aPosition.myY + m_drawOffsetY);
}

GameEntity::~GameEntity(void)
{
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{

	return false;	
}