#include "Ghost.h"

Ghost::Ghost(const Vector2f& aPosition)
: MovableGameEntity(aPosition, "ghost_32.png")
{
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;
}

Ghost::~Ghost(void)
{
	while (!myPath.empty()) delete myPath.front(), myPath.pop_front();
}

void Ghost::Die(World* aWorld)
{
	myPath.clear();
	aWorld->GetPath(m_currentTile.myX, m_currentTile.myY, 13, 13, myPath);
}

void Ghost::Reset()
{
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;

	m_currentTile = m_nextTile = Vector2f(myPosition.myX / 22, myPosition.myY / 22);
}

void Ghost::Update(float aTime, World* aWorld)
{
	float speed = 30.f;
	int nextTileX = m_currentTile.myX + myDesiredMovementX;
	int nextTileY = m_currentTile.myY + myDesiredMovementY;

	if (myIsDeadFlag)
		speed = 120.f;

	if (IsAtDestination())
	{
		if (!myPath.empty())
		{
			PathmapTile* nextTile = myPath.front();
			myPath.pop_front();
			SetNextTile(nextTile->m_pos);
		}
		else if (aWorld->TileIsValid(nextTileX, nextTileY))
		{
			SetNextTile(Vector2f(nextTileX, nextTileY));
		}
		else
		{
			if (myDesiredMovementX == 1)
			{
				myDesiredMovementX = 0;
				myDesiredMovementY = 1;
			} else if (myDesiredMovementY == 1)
			{
				myDesiredMovementX = -1;
				myDesiredMovementY = 0;			
			} else if (myDesiredMovementX == -1)
			{
				myDesiredMovementX = 0;
				myDesiredMovementY = -1;
			} else
			{
				myDesiredMovementX = 1;
				myDesiredMovementY = 0;
			}

			myIsDeadFlag = false;
		}
	}

	int tileSize = 22;
	Vector2f destination(m_nextTile.myX * tileSize, m_nextTile.myY * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * speed;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		m_currentTile = m_nextTile;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
	SetDrawPos(Vector2f(myPosition.myX + m_drawOffsetX, myPosition.myY + m_drawOffsetY));
}
