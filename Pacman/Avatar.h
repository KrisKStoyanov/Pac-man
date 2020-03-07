#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition, float movementSpeed, int tileSize);
	~Avatar(void);

	inline Vector2f GetSpawnPos() { return m_spawnPos; }
private:
	Vector2f m_spawnPos;
};

#endif //AVATAR_H