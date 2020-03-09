#include "Ghost.h"

Ghost::Ghost(
	const Vector2f& aPosition, 
	const float chaseSpeed,
	const float scatterSpeed,
	const float frightenedSpeed,
	const float chaseDuration,
	const float frightenedDuration,
	const float scatterDuration,
	const Vector2f& nextDirection) :
	MovableGameEntity(aPosition, chaseSpeed), 
	m_spawnPos(aPosition),
	m_nextDir(nextDirection),
	m_chaseSpeed(chaseSpeed),
	m_scatterSpeed(scatterSpeed),
	m_frightenedSpeed(frightenedSpeed),
	m_chaseDefaultDuration(chaseDuration),
	m_chaseCurrentDuration(chaseDuration),
	m_frightenedCurrentDuration(frightenedDuration),
	m_frightenedDefaultDuration(frightenedDuration),
	m_scatterDefaultDuration(scatterDuration),
	m_scatterCurrentDuration(scatterDuration),
	m_stateDurationReducer(1.0f)
{
	m_state = GhostState::CHASE;
	myIsDeadFlag = false;
}

Ghost::~Ghost(void)
{

}

void Ghost::SetState(GhostState state)
{
	switch (state)
	{
	case GhostState::CHASE:
		m_movementSpeed = m_chaseSpeed;
		m_stateDuration = m_chaseDefaultDuration;
		break;
	case GhostState::FRIGHTENED:
		m_movementSpeed = m_frightenedSpeed;
		m_stateDuration = m_frightenedDefaultDuration;
		break;
	case GhostState::SCATTER:
		m_movementSpeed = m_scatterSpeed;
		m_stateDuration = m_scatterDefaultDuration;
		break;
	default:
		break;
	}
	m_state = state;
}

void Ghost::Update(
	World& world, 
	const Vector2f& target, 
	const float deltaTime, 
	const float tileSize, 
	const Vector2f& drawOffset)
{
	float distanceToMove = deltaTime * m_movementSpeed;
	Vector2f direction = m_nextTile - m_currentTile;

	if (distanceToMove > direction.Length())
	{
		m_currentTile = m_nextTile;
		Vector2f nextDirTile = m_currentTile + m_nextDir * tileSize;

		switch (m_state)
		{
		case GhostState::CHASE:
		{
			if (world.TileIsValid(nextDirTile))
			{
				float distToAvatarDir = (nextDirTile - target).Length();

				if (m_nextDir.myX != 0)
				{
					Vector2f nextUpTile = m_currentTile + world.m_upDir * tileSize;
					Vector2f nextDownTile = m_currentTile + world.m_downDir * tileSize;

					float distToAvatarUp = distToAvatarDir + 1.0f;
					float distToAvatarDown = distToAvatarDir + 1.0f;

					if (world.TileIsValid(nextUpTile))
					{
						distToAvatarUp = (nextUpTile - target).Length();
					}
					if (world.TileIsValid(nextDownTile))
					{
						distToAvatarDown = (nextDownTile - target).Length();
					}

					if (distToAvatarDir < distToAvatarUp && distToAvatarDir < distToAvatarDown)
					{
						m_nextTile = nextDirTile;
					}
					else if (distToAvatarUp < distToAvatarDown)
					{
						m_nextDir = world.m_upDir;
						m_nextTile = nextUpTile;
					}
					else
					{
						m_nextDir = world.m_downDir;
						m_nextTile = nextDownTile;
					}
				}
				else if (m_nextDir.myY != 0)
				{
					Vector2f nextLeftTile = m_currentTile + world.m_leftDir * tileSize;
					Vector2f nextRightTile = m_currentTile + world.m_rightDir * tileSize;

					float distToAvatarLeft = distToAvatarDir + 1.0f;
					float distToAvatarRight = distToAvatarDir + 1.0f;

					if (world.TileIsValid(nextLeftTile))
					{
						distToAvatarLeft = (nextLeftTile - target).Length();
					}
					if (world.TileIsValid(nextRightTile))
					{
						distToAvatarRight = (nextRightTile - target).Length();
					}

					if (distToAvatarDir < distToAvatarLeft && distToAvatarDir < distToAvatarRight)
					{
						m_nextTile = nextDirTile;
					}
					else if (distToAvatarLeft < distToAvatarRight)
					{
						m_nextDir = world.m_leftDir;
						m_nextTile = nextLeftTile;
					}
					else
					{
						m_nextDir = world.m_rightDir;
						m_nextTile = nextRightTile;
					}
				}
			}
			else if (m_nextDir.myX != 0)
			{
				Vector2f nextUpTile = m_currentTile + world.m_upDir * tileSize;
				Vector2f nextDownTile = m_currentTile + world.m_downDir * tileSize;

				if (world.TileIsValid(nextUpTile) && world.TileIsValid(nextDownTile))
				{
					if ((nextUpTile - target).Length() < (nextDownTile - target).Length())
					{
						m_nextDir = world.m_upDir;
						m_nextTile = nextUpTile;
					}
					else
					{
						m_nextDir = world.m_downDir;
						m_nextTile = nextDownTile;
					}
				}
				else if (world.TileIsValid(nextUpTile))
				{
					m_nextDir = world.m_upDir;
					m_nextTile = nextUpTile;
				}
				else
				{
					m_nextDir = world.m_downDir;
					m_nextTile = nextDownTile;
				}
			}
			else if (m_nextDir.myY != 0)
			{
				Vector2f nextLeftTile = m_currentTile + world.m_leftDir * tileSize;
				Vector2f nextRightTile = m_currentTile + world.m_rightDir * tileSize;

				if (world.TileIsValid(nextLeftTile) && world.TileIsValid(nextRightTile))
				{
					if ((nextLeftTile - target).Length() < (nextRightTile - target).Length())
					{
						m_nextDir = world.m_leftDir;
						m_nextTile = nextLeftTile;
					}
					else
					{
						m_nextDir = world.m_rightDir;
						m_nextTile = nextRightTile;
					}
				}
				else if (world.TileIsValid(nextLeftTile))
				{
					m_nextDir = world.m_leftDir;
					m_nextTile = nextLeftTile;
				}
				else
				{
					m_nextDir = world.m_rightDir;
					m_nextTile = nextRightTile;
				}
			}
			myIsDeadFlag = false;
		}
		break;
		case GhostState::FRIGHTENED:
		{
			if (world.TileIsValid(nextDirTile))
			{
				float distToAvatarDir = (nextDirTile - target).Length();

				if (m_nextDir.myX != 0)
				{
					Vector2f nextUpTile = m_currentTile + world.m_upDir * tileSize;
					Vector2f nextDownTile = m_currentTile + world.m_downDir * tileSize;

					float distToAvatarUp = distToAvatarDir - 1.0f;
					float distToAvatarDown = distToAvatarDir - 1.0f;

					if (world.TileIsValid(nextUpTile))
					{
						distToAvatarUp = (nextUpTile - target).Length();
					}
					if (world.TileIsValid(nextDownTile))
					{
						distToAvatarDown = (nextDownTile - target).Length();
					}

					if (distToAvatarDir > distToAvatarUp && distToAvatarDir > distToAvatarDown)
					{
						m_nextTile = nextDirTile;
					}
					else if (distToAvatarUp > distToAvatarDown)
					{
						m_nextDir = world.m_upDir;
						m_nextTile = nextUpTile;
					}
					else
					{
						m_nextDir = world.m_downDir;
						m_nextTile = nextDownTile;
					}
				}
				else if (m_nextDir.myY != 0)
				{
					Vector2f nextLeftTile = m_currentTile + world.m_leftDir * tileSize;
					Vector2f nextRightTile = m_currentTile + world.m_rightDir * tileSize;

					float distToAvatarLeft = distToAvatarDir - 1.0f;
					float distToAvatarRight = distToAvatarDir - 1.0f;

					if (world.TileIsValid(nextLeftTile))
					{
						distToAvatarLeft = (nextLeftTile - target).Length();
					}
					if (world.TileIsValid(nextRightTile))
					{
						distToAvatarRight = (nextRightTile - target).Length();
					}

					if (distToAvatarDir > distToAvatarLeft && distToAvatarDir > distToAvatarRight)
					{
						m_nextTile = nextDirTile;
					}
					else if (distToAvatarLeft > distToAvatarRight)
					{
						m_nextDir = world.m_leftDir;
						m_nextTile = nextLeftTile;
					}
					else
					{
						m_nextDir = world.m_rightDir;
						m_nextTile = nextRightTile;
					}
				}
			}
			else if (m_nextDir.myX != 0)
			{
				Vector2f nextUpTile = m_currentTile + world.m_upDir * tileSize;
				Vector2f nextDownTile = m_currentTile + world.m_downDir * tileSize;

				if (world.TileIsValid(nextUpTile) && world.TileIsValid(nextDownTile))
				{
					if ((nextUpTile - target).Length() > (nextDownTile - target).Length())
					{
						m_nextDir = world.m_upDir;
						m_nextTile = nextUpTile;
					}
					else
					{
						m_nextDir = world.m_downDir;
						m_nextTile = nextDownTile;
					}
				}
				else if (world.TileIsValid(nextUpTile))
				{
					m_nextDir = world.m_upDir;
					m_nextTile = nextUpTile;
				}
				else
				{
					m_nextDir = world.m_downDir;
					m_nextTile = nextDownTile;
				}
			}
			else if (m_nextDir.myY != 0)
			{
				Vector2f nextLeftTile = m_currentTile + world.m_leftDir * tileSize;
				Vector2f nextRightTile = m_currentTile + world.m_rightDir * tileSize;

				if (world.TileIsValid(nextLeftTile) && world.TileIsValid(nextRightTile))
				{
					if ((nextLeftTile - target).Length() > (nextRightTile - target).Length())
					{
						m_nextDir = world.m_leftDir;
						m_nextTile = nextLeftTile;
					}
					else
					{
						m_nextDir = world.m_rightDir;
						m_nextTile = nextRightTile;
					}
				}
				else if (world.TileIsValid(nextLeftTile))
				{
					m_nextDir = world.m_leftDir;
					m_nextTile = nextLeftTile;
				}
				else
				{
					m_nextDir = world.m_rightDir;
					m_nextTile = nextRightTile;
				}
			}
			myIsDeadFlag = false;
		}
		break;
		case GhostState::SCATTER:
		{
			if (world.TileIsValid(nextDirTile))
			{
				if (m_nextDir.myX != 0)
				{
					Vector2f nextUpTile = m_currentTile + world.m_upDir * tileSize;
					Vector2f nextDownTile = m_currentTile + world.m_downDir * tileSize;

					float r = world.GetRandFloat(deltaTime);

					if (world.TileIsValid(nextUpTile) && world.TileIsValid(nextDownTile))
					{
						if (r < 3.3f)
						{
							m_nextDir = world.m_upDir;
							m_nextTile = nextUpTile;
						}
						else if (r < 6.6f)
						{
							m_nextDir = world.m_downDir;
							m_nextTile = nextDownTile;
						}
						else
						{
							m_nextTile = nextDirTile;
						}

					}
					else if (world.TileIsValid(nextUpTile))
					{
						if (r < 5.0f)
						{
							m_nextDir = world.m_upDir;
							m_nextTile = nextUpTile;
						}
						else
						{
							m_nextTile = nextDirTile;
						}
					}
					else if (world.TileIsValid(nextDownTile))
					{
						if (r < 5.0f)
						{
							m_nextDir = world.m_downDir;
							m_nextTile = nextDownTile;
						}
						else
						{
							m_nextTile = nextDirTile;
						}
					}
					else
					{
						m_nextTile = nextDirTile;
					}
				}
				else if (m_nextDir.myY != 0)
				{
					Vector2f nextLeftTile = m_currentTile + world.m_leftDir * tileSize;
					Vector2f nextRightTile = m_currentTile + world.m_rightDir * tileSize;

					float r = world.GetRandFloat(deltaTime);

					if (world.TileIsValid(nextLeftTile) && world.TileIsValid(nextRightTile))
					{
						if (r < 3.3f)
						{
							m_nextDir = world.m_leftDir;
							m_nextTile = nextLeftTile;
						}
						else if (r < 6.6f)
						{
							m_nextDir = world.m_rightDir;
							m_nextTile = nextRightTile;
						}
						else
						{
							m_nextTile = nextDirTile;
						}

					}
					else if (world.TileIsValid(nextLeftTile))
					{
						if (r < 5.0f)
						{
							m_nextDir = world.m_leftDir;
							m_nextTile = nextLeftTile;
						}
						else
						{
							m_nextTile = nextDirTile;
						}
					}
					else if (world.TileIsValid(nextRightTile))
					{
						if (r < 5.0f)
						{
							m_nextDir = world.m_rightDir;
							m_nextTile = nextRightTile;
						}
						else
						{
							m_nextTile = nextDirTile;
						}
					}
					else
					{
						m_nextTile = nextDirTile;
					}
				}
			}
			else if (m_nextDir.myX != 0)
			{
				Vector2f nextUpTile = m_currentTile + world.m_upDir * tileSize;
				Vector2f nextDownTile = m_currentTile + world.m_downDir * tileSize;

				if (world.TileIsValid(nextUpTile) && world.TileIsValid(nextDownTile))
				{
					float r = world.GetRandFloat(deltaTime);
					if (r < 5.0f)
					{
						m_nextDir = world.m_upDir;
						m_nextTile = nextUpTile;
					}
					else
					{
						m_nextDir = world.m_downDir;
						m_nextTile = nextDownTile;
					}
				}
				else if (world.TileIsValid(nextUpTile))
				{
					m_nextDir = world.m_upDir;
					m_nextTile = nextUpTile;
				}
				else
				{
					m_nextDir = world.m_downDir;
					m_nextTile = nextDownTile;
				}
			}
			else if (m_nextDir.myY != 0)
			{
				Vector2f nextLeftTile = m_currentTile + world.m_leftDir * tileSize;
				Vector2f nextRightTile = m_currentTile + world.m_rightDir * tileSize;

				if (world.TileIsValid(nextLeftTile) && world.TileIsValid(nextRightTile))
				{
					float r = world.GetRandFloat(deltaTime);
					if (r < 5.0f)
					{
						m_nextDir = world.m_leftDir;
						m_nextTile = nextLeftTile;
					}
					else
					{
						m_nextDir = world.m_rightDir;
						m_nextTile = nextRightTile;
					}
				}
				else if (world.TileIsValid(nextLeftTile))
				{
					m_nextDir = world.m_leftDir;
					m_nextTile = nextLeftTile;
				}
				else
				{
					m_nextDir = world.m_rightDir;
					m_nextTile = nextRightTile;
				}
			}
			myIsDeadFlag = false;
		}
		break;
		default:
			break;
		}
	}
	else
	{
		m_direction = direction.Normalize();
		m_currentTile += direction * distanceToMove;
	}

	myPosition = m_currentTile;
	m_drawPos = myPosition + drawOffset;

	m_stateDuration -= deltaTime * m_stateDurationReducer;

	if (m_stateDuration < 0.0f)
	{
		switch (m_state)
		{
		case GhostState::CHASE:
		{
			SetState(GhostState::SCATTER);
		}
		break;
		case GhostState::FRIGHTENED:
		{
			SetState(GhostState::SCATTER);
		}
		break;
		case GhostState::SCATTER:
		{
			SetState(GhostState::CHASE);
		}
		break;
		default:
		break;
		}
	}
}

void Ghost::Draw(Renderer& renderer, DrawEntity& defaultImage, DrawEntity& vulnerableImage, DrawEntity& deadImage)
{
	if (myIsDeadFlag)
	{
		renderer.DrawObject(deadImage, m_drawPos.myX, m_drawPos.myY);
	}
	else if (m_state == GhostState::FRIGHTENED)
	{
		renderer.DrawObject(vulnerableImage, m_drawPos.myX, m_drawPos.myY);
	}
	else
	{
		renderer.DrawObject(defaultImage, m_drawPos.myX, m_drawPos.myY);
	}
}
