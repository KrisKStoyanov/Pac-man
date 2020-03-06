#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition);
	~Avatar(void);

	void Update(float aTime);

	inline Vector2f GetDrawPos()
	{
		return m_drawPos;
	}
};

#endif //AVATAR_H