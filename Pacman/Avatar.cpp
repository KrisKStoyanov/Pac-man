#include "Avatar.h"

Avatar::Avatar(const Vector2f& aPosition, float movementSpeed) : 
	MovableGameEntity(aPosition, movementSpeed),
	m_spawnPos(aPosition), 
	m_nextDir(-1.0f, 0.0f),
	m_toggleDrawCooldown(0.05f),
	m_toggleDrawDefault(0.05f),
	m_toggleDrawReducer(1.0f)
{

}

Avatar::~Avatar(void)
{
}

void Avatar::Update(World& world, float deltaTime, float tileSize, Vector2f& drawOffset)
{
	float distanceToMove = deltaTime * m_movementSpeed;
	Vector2f direction = m_nextTile - m_currentTile;

	if (distanceToMove > direction.Length())
	{
		m_currentTile = m_nextTile;

		Vector2f nextInputTile = m_currentTile + m_nextDir * tileSize;
		Vector2f nextDirTile = m_currentTile + m_direction * tileSize;

		if (world.TileIsValid(nextInputTile))
		{
			m_nextTile = nextInputTile;
		}
		else if (world.TileIsValid(nextDirTile))
		{
			m_nextTile = nextDirTile;
		}
		m_toggleDrawCooldown = m_toggleDrawDefault;
	}
	else
	{
		m_direction = direction.Normalize();
		m_currentTile += direction * distanceToMove;

		m_toggleDrawCooldown -= deltaTime * m_toggleDrawReducer;
		if (m_toggleDrawCooldown < 0.0f)
		{
			m_drawOpen = !m_drawOpen;
			m_toggleDrawCooldown = m_toggleDrawDefault;
		}
	}

	myPosition = m_currentTile;
	m_drawPos = m_currentTile + drawOffset;
}

void Avatar::Draw(Renderer& renderer, 
	DrawEntity& leftOpenImage, 
	DrawEntity& rightOpenImage, 
	DrawEntity& upOpenImage, 
	DrawEntity& downOpenImage, 
	DrawEntity& leftClosedImage, 
	DrawEntity& rightClosedImage, 
	DrawEntity& upClosedImage, 
	DrawEntity& downClosedImage)
{
	if (m_direction == Vector2f(1, 0))
	{
		if (m_drawOpen)
		{
			renderer.DrawObject(rightOpenImage, m_drawPos.myX, m_drawPos.myY);
		}
		else
		{
			renderer.DrawObject(rightClosedImage, m_drawPos.myX, m_drawPos.myY);
		}
	}
	else if (m_direction == Vector2f(-1, 0))
	{
		if (m_drawOpen)
		{
			renderer.DrawObject(leftOpenImage, m_drawPos.myX, m_drawPos.myY);
		}
		else
		{
			renderer.DrawObject(leftClosedImage, m_drawPos.myX, m_drawPos.myY);
		}
	}
	else if (m_direction == Vector2f(0, 1))
	{
		if (m_drawOpen)
		{
			renderer.DrawObject(downOpenImage, m_drawPos.myX, m_drawPos.myY);
		}
		else
		{
			renderer.DrawObject(downClosedImage, m_drawPos.myX, m_drawPos.myY);
		}
	}
	else if (m_direction == Vector2f(0, -1))
	{
		if (m_drawOpen)
		{
			renderer.DrawObject(upOpenImage, m_drawPos.myX, m_drawPos.myY);
		}
		else
		{
			renderer.DrawObject(upClosedImage, m_drawPos.myX, m_drawPos.myY);
		}
	}
}


