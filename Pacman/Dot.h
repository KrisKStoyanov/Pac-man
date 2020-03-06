#ifndef DOT_H
#define DOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class Dot : public StaticGameEntity
{
public:
	Dot(Vector2f aPosition);
	~Dot(void);

	inline Vector2f GetDrawPos()
	{
		return m_drawPos;
	}
private:
	Vector2f m_drawPos;
	float m_offsetX = 220;
	float m_offsetY = 60;
};

#endif // DOT_H