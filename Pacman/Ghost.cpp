#include "Ghost.h"

Ghost::Ghost(const Vector2f& aPosition, float movementSpeed) : 
	MovableGameEntity(aPosition, movementSpeed), 
	m_spawnPos(aPosition)
{
	myIsDeadFlag = false;
	m_desiredMovement = Vector2f(0, -1);
}

Ghost::~Ghost(void)
{
	SAFE_DELETE_VECTOR(myPath);
}