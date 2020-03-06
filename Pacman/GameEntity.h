#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2f.h"
#include "DrawTextEntity.h"

class Core;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition, const char* anImage);
	~GameEntity(void);

	inline Vector2f GetPosition() const { return myPosition; }
	inline void SetPosition(const Vector2f& aPosition) { myPosition = aPosition; }

	inline Vector2f GetDrawPos() { return m_drawPos; }
	inline void SetDrawPos(const Vector2f& drawPos) { m_drawPos = drawPos; }

	bool Intersect(GameEntity* aGameEntity);

	inline void MarkForDelete() { myIdMarkedForDeleteFlag = true; }
	inline bool IsMarkedForDelete() const { return myIdMarkedForDeleteFlag; }

protected:

	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition;
	const char* myImage;

	Vector2f m_drawPos;
	const float m_drawOffsetX;
	const float m_drawOffsetY;
};

#endif // GAMEENTITY_H