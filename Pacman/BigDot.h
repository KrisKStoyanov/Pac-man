#ifndef BIGDOT_H
#define BIGDOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class BigDot : public StaticGameEntity
{
public:
	BigDot(Vector2f aPosition);
	~BigDot(void);

	inline Vector2f GetDrawPos() 
	{
		return m_drawPos;
	}
private:
	Vector2f m_drawPos;
	float m_offsetX = 220;
	float m_offsetY = 60;
};

#endif // BIGDOT_H