#include "PathmapTile.h"

PathmapTile::PathmapTile(int anX, int anY, bool aIsBlockingFlag)
: m_pos(anX, anY)
, myIsBlockingFlag(aIsBlockingFlag)
,myIsVisitedFlag(false)
{
}

PathmapTile::~PathmapTile(void)
{
}
