#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

#include "Vector2f.h"
class PathmapTile
{
public:
	PathmapTile(int anX, int anY, bool aIsBlockingFlag);
	~PathmapTile(void);

	Vector2f m_pos;
	bool myIsBlockingFlag;
	bool myIsVisitedFlag;

};

#endif // PATHMAPTILE_H