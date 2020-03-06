#ifndef GHOST_H
#define GHOST_H

#include "World.h"
#include "PathmapTile.h"
#include <list>
#include "MovableGameEntity.h"

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition);
	~Ghost(void);

	void Update(float aTime, World* aWorld);

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void Die(World* aWorld);

	inline Vector2f GetDrawPos()
	{
		return m_drawPos;
	}
protected:
	int myDesiredMovementX;
	int myDesiredMovementY;
	std::list<PathmapTile*> myPath;
};

#endif // GHOST_H