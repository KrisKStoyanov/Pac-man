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

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void Die(World* aWorld);

	void Reset();

	inline Vector2f GetDrawPos() { return m_drawPos; }
	inline Vector2f GetDesiredMovement() { return m_desiredMovement; }
	inline void SetDesiredMovement(const Vector2f& movement) { m_desiredMovement = movement; }
	inline std::vector<PathmapTile*> GetPath() { return myPath; }
protected:
	Vector2f m_desiredMovement;
	std::vector<PathmapTile*> myPath;
};

#endif // GHOST_H