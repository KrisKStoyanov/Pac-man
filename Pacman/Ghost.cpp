#include "Ghost.h"

Ghost::Ghost(
	const Vector2f& aPosition, 
	const float chaseSpeed,
	const float scatterSpeed,
	const float frightenedSpeed,
	const Vector2f& direction) :
	MovableGameEntity(aPosition, chaseSpeed), 
	m_spawnPos(aPosition),
	m_nextDir(direction),
	m_chaseSpeed(chaseSpeed),
	m_scatterSpeed(scatterSpeed),
	m_frightenedSpeed(frightenedSpeed)
{
	m_state = GhostState::CHASE;
	myIsDeadFlag = false;
}

Ghost::~Ghost(void)
{
	SAFE_DELETE_VECTOR(myPath);
}

void Ghost::TransitionState(GhostState state)
{
	switch (state)
	{
	case GhostState::CHASE:
	{
		m_movementSpeed = m_chaseSpeed;
	}
	break;
	case GhostState::SCATTER:
	{
		m_movementSpeed = m_scatterSpeed;
	}
	break;
	case GhostState::FRIGHTENED:
	{
		m_movementSpeed = m_frightenedSpeed;
	}
	break;
	}

	m_state = state;
}
