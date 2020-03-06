#include "Avatar.h"

Avatar::Avatar(const Vector2f& aPosition, float movementSpeed)
: MovableGameEntity(aPosition, "open_32.png", movementSpeed)
{

}

Avatar::~Avatar(void)
{
}

void Avatar::Update(float aTime)
{
	int tileSize = 22;

	Vector2f destination(m_nextTile * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * m_movementSpeed;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		m_currentTile = m_nextTile;
	}
	else
	{
		m_direction = direction.Normalize();
		myPosition += direction * distanceToMove;
	}

	m_drawPos = Vector2f(myPosition.myX + m_drawOffsetX, myPosition.myY + m_drawOffsetY);
}

void Avatar::Reset()
{
	m_currentTile = m_nextTile = Vector2f(myPosition.myX / 22, myPosition.myY / 22);
}
