#include "Avatar.h"

Avatar::Avatar(const Vector2f& aPosition, float movementSpeed) : 
	MovableGameEntity(aPosition, movementSpeed),
	m_spawnPos(aPosition)
{

}

Avatar::~Avatar(void)
{
}


