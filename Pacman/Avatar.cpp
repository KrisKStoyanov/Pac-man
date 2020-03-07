#include "Avatar.h"

Avatar::Avatar(const Vector2f& aPosition, float movementSpeed, int tileSize) : 
	MovableGameEntity(aPosition, movementSpeed, tileSize),
	m_spawnPos(aPosition)
{

}

Avatar::~Avatar(void)
{
}


