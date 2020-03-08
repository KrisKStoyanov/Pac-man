#include "Ghost.h"

Ghost::Ghost(
	const Vector2f& aPosition, 
	const float movementSpeed,
	const Vector2f& desiredMovement) :
	MovableGameEntity(aPosition, movementSpeed), 
	m_spawnPos(aPosition),
	m_desiredMovement(desiredMovement)
{
	myIsDeadFlag = false;
}

Ghost::~Ghost(void)
{
	SAFE_DELETE_VECTOR(myPath);
}