#include "Ghost.h"

Ghost::Ghost(const Vector2f& aPosition, float movementSpeed)
: MovableGameEntity(aPosition, "ghost_32.png", movementSpeed)
{
	myIsDeadFlag = false;

	m_desiredMovement = Vector2f(0, -1);
}

Ghost::~Ghost(void)
{
	while (!myPath.empty()) delete myPath.back(), myPath.pop_back();
}

void Ghost::Die(World* aWorld)
{
	aWorld->GetPath(m_currentTile, Vector2f(13, 13), myPath);
}

void Ghost::Reset()
{
	myPath.clear();
	m_desiredMovement = Vector2f(0, -1);

	m_currentTile = m_nextTile = Vector2f(myPosition.myX / 22, myPosition.myY / 22);
}
