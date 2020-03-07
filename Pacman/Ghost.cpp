#include "Ghost.h"

Ghost::Ghost(const Vector2f& aPosition, float movementSpeed)
: MovableGameEntity(aPosition, movementSpeed)
{
	myIsDeadFlag = false;

	m_desiredMovement = Vector2f(0, -1);
}

Ghost::~Ghost(void)
{
	while (!myPath.empty()) delete myPath.back(), myPath.pop_back();
}