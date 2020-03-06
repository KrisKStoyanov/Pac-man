#include "Dot.h"

Dot::Dot(Vector2f aPosition)
: StaticGameEntity(aPosition, "Small_Dot_32.png")
{
	m_drawPos.myX = aPosition.myX + m_offsetX;
	m_drawPos.myY = aPosition.myY + m_offsetY;
}

Dot::~Dot(void)
{
}
