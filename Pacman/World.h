#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "Core.h"

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Cherry.h"

struct WORLD_DESC
{
	const char* playfieldImage = "playfield.png";
	const char* dotImage = "Small_Dot_32.png";
	const char* bigDotImage = "Big_Dot_32.png";
	const char* cherryImage = "dot.png";
};

class World
{
public:
	World(void);
	~World(void);

	void Init(Core& core, WORLD_DESC& world_desc);

	void Draw(Core& core);
	bool TileIsValid(int anX, int anY);

	bool HasIntersectedDot(const Vector2f& aPosition);
	bool HasIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);

	void Update(Core* core);

	void GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<PathmapTile*>& aList);
private:

	PathmapTile* GetTile(int aFromX, int aFromY);
	bool Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList);
	bool ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList);


	bool InitPathmap();

	std::vector<PathmapTile*> myPathmapTiles;
	std::vector<Dot*> myDots;
	std::vector<BigDot*> myBigDots;
	std::vector<Cherry*> myCherry;

	DrawEntity* m_pPlayfield;

	DrawEntity* m_pDot;
	DrawEntity* m_pBigDot;

	WORLD_DESC world_desc;
};

#endif // WORLD_H