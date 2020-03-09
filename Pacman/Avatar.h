#ifndef AVATAR_H
#define AVATAR_H

#include "World.h"
#include "MovableGameEntity.h"
#include "Vector2f.h"

struct AVATAR_DRAW_DESC
{

};

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition, float movementSpeed);
	~Avatar(void);

	inline Vector2f GetSpawnPos() { return m_spawnPos; }

	inline Vector2f GetNextDir() { return m_nextDir; }
	inline void SetNextDir(const Vector2f& direction) { m_nextDir = direction; }

	void Update(World& world, float deltaTime, float tileSize, Vector2f& drawOffset);
	void Draw(
		Renderer& renderer,
		DrawEntity& leftOpenImage,
		DrawEntity& rightOpenImage,
		DrawEntity& upOpenImage,
		DrawEntity& downOpenImage,
		DrawEntity& leftClosedImage,
		DrawEntity& rightClosedImage,
		DrawEntity& upClosedImage,
		DrawEntity& downClosedImage);

private:
	
	Vector2f m_nextDir;
	Vector2f m_spawnPos;

	bool m_drawOpen;

	float m_toggleDrawCooldown;
	float m_toggleDrawDefault;
	float m_toggleDrawReducer;

};

#endif //AVATAR_H