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
	DrawEntity* playfieldDrawEntity;
	DrawEntity* dotDrawEntity;
	DrawEntity* bigDotDrawEntity;
	DrawEntity* cherryDrawEntity;
};

class World
{
public:
	World(void);
	~World(void);

	void Init(const WORLD_DESC& world_desc);

	void Draw(Renderer& renderer);
	bool TileIsValid(Vector2f tilePos);

	bool HasIntersectedDot(GameEntity& gameEntity, bool& win);
	bool HasIntersectedBigDot(GameEntity& gameEntity, bool& win);
	bool HasIntersectedCherry(GameEntity& gameEntity);

	void Shutdown();

	std::vector<PathmapTile*> GetPath(Vector2f fromTile, Vector2f toTile);
private:

	PathmapTile* GetTile(Vector2f tilePos);
	bool Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::vector<PathmapTile*>& aList);
	bool ListDoesNotContain(PathmapTile* aFromTile, std::vector<PathmapTile*>& aList);


	bool InitPathmap();

	std::vector<PathmapTile*> myPathmapTiles;
	std::vector<Dot*> myDots;
	std::vector<BigDot*> myBigDots;
	std::vector<Cherry*> myCherry;

	float m_dotIntersectionDist;

	WORLD_DESC m_desc;
};

#endif // WORLD_H