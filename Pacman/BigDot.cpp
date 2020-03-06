#include "BigDot.h"

BigDot::BigDot(Vector2f aPosition)
: StaticGameEntity(aPosition, "Big_Dot_32.png")
{
	m_drawPos.myX = aPosition.myX + m_offsetX;
	m_drawPos.myY = aPosition.myY + m_offsetY;
}

BigDot::~BigDot(void)
{
}
