#ifndef GHOST_H
#define GHOST_H

#include "PathmapTile.h"
#include <vector>
#include "MovableGameEntity.h"

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition, float movementSpeed, int tileSize);
	~Ghost(void);
	inline bool GetDeadFlag() { return myIsDeadFlag; }
	inline void SetDeadFlag(bool flag) { myIsDeadFlag = flag; }
	inline Vector2f GetDrawPos() { return m_drawPos; }
	inline Vector2f GetDesiredMovement() { return m_desiredMovement; }
	inline void SetDesiredMovement(const Vector2f& movement) { m_desiredMovement = movement; }
	inline void SetPath(const std::vector<PathmapTile*>& path) { myPath = path; }
	inline std::vector<PathmapTile*> GetPath() { return myPath; }
	inline Vector2f GetSpawnPos() { return m_spawnPos; }
private:
	bool myIsDeadFlag;
	Vector2f m_desiredMovement;
	std::vector<PathmapTile*> myPath;

	Vector2f m_spawnPos;
};

#endif // GHOST_H