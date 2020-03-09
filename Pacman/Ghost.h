#ifndef GHOST_H
#define GHOST_H

#include "World.h"
#include <vector>
#include <list>
#include "MovableGameEntity.h"

enum class GhostState : unsigned short
{
	CHASE = 0,
	SCATTER,
	FRIGHTENED
};

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition, 
		const float chaseSpeed, 
		const float scatterSpeed, 
		const float frightenedSpeed,
		const float chaseDuration,
		const float frightenedDuration,
		const float scatterDuration,
		const Vector2f& nextDirection);
	~Ghost(void);
	inline bool GetDeadFlag() { return myIsDeadFlag; }
	inline void SetDeadFlag(bool flag) { myIsDeadFlag = flag; }
	inline Vector2f GetDrawPos() { return m_drawPos; }
	inline void SetNextDir(const Vector2f& direction) { m_nextDir = direction; }
	inline Vector2f GetSpawnPos() { return m_spawnPos; }
	inline GhostState GetState() { return m_state; }
	void SetState(GhostState state);

	inline const float GetChaseSpeed() { return m_chaseSpeed; }
	inline const float GetScatterSpeed() { return m_scatterSpeed; }
	inline const float GetFrightenedSpeed() { return m_frightenedSpeed; }

	void Update(World& world, const Vector2f& target, const float deltaTime, const float tileSize, const Vector2f& drawOffset);
	void Draw(Renderer& renderer, DrawEntity& defaultImage, DrawEntity& vulnerableImage, DrawEntity& deadImage);

private:

	bool myIsDeadFlag;
	Vector2f m_nextDir;

	Vector2f m_spawnPos;
	GhostState m_state;

	float m_chaseSpeed;
	float m_scatterSpeed;
	float m_frightenedSpeed;

	float m_chaseDefaultDuration;
	float m_chaseCurrentDuration;
	
	float m_scatterDefaultDuration;
	float m_scatterCurrentDuration;

	float m_frightenedDefaultDuration;
	float m_frightenedCurrentDuration;

	float m_stateDurationReducer;

	float m_stateDuration;
};

#endif // GHOST_H