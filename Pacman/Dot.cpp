#include "Dot.h"

Dot::Dot(Vector2f aPosition)
: StaticGameEntity(aPosition, "Small_Dot_32.png")
{
	myPosition.myX = aPosition.myX + m_offsetX;
	myPosition.myY = aPosition.myY + m_offsetY;
}

Dot::~Dot(void)
{
}
