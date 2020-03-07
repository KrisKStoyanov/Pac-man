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
	bool TileIsValid(Vector2f tilePos);

	bool HasIntersectedDot(const Vector2f& aPosition, bool& win);
	bool HasIntersectedBigDot(const Vector2f& aPosition, bool& win);
	bool HasIntersectedCherry(const Vector2f& aPosition);

	void Shutdown();

	void GetPath(Vector2f fromTile, Vector2f toTile, std::vector<PathmapTile*>& aList);
private:

	PathmapTile* GetTile(Vector2f tilePos);
	bool Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::vector<PathmapTile*>& aList);
	bool ListDoesNotContain(PathmapTile* aFromTile, std::vector<PathmapTile*>& aList);


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