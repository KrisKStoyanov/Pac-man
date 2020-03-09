#include "PathmapTile.h"

PathmapTile::PathmapTile(Vector2f pos, bool aIsBlockingFlag, bool teleport) : 
	m_pos(pos),
	myIsBlockingFlag(aIsBlockingFlag), 
	myIsVisitedFlag(false), 
	m_teleport(teleport)
{

}

PathmapTile::~PathmapTile(void)
{
}
