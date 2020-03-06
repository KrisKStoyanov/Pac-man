#ifndef GHOST_H
#define GHOST_H

#include "World.h"
#include "PathmapTile.h"
#include <vector>
#include "MovableGameEntity.h"

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition, float movementSpeed);
	~Ghost(void);

	void Update(float aTime, World* aWorld);

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void Die(World* aWorld);

	void Reset();

	inline Vector2f GetDrawPos() { return m_drawPos; }
protected:
	int myDesiredMovementX;
	int myDesiredMovementY;
	std::vector<PathmapTile*> myPath;
};

#endif // GHOST_H