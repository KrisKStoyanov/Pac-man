#include "PathmapTile.h"

PathmapTile::PathmapTile(Vector2f pos, bool aIsBlockingFlag) : 
	m_pos(pos),
	myIsBlockingFlag(aIsBlockingFlag), 
	myIsVisitedFlag(false)
{

}

PathmapTile::~PathmapTile(void)
{
}
