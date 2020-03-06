#ifndef DOT_H
#define DOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class Dot : public StaticGameEntity
{
public:
	Dot(Vector2f aPosition);
	~Dot(void);
private:
	float m_offsetX = 220;
	float m_offsetY = 60;
};

#endif // DOT_H