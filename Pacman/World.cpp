#include "World.h"

World::World(void) : m_dotIntersectionDist(5.0f)
{
}

World::~World(void)
{
}

void World::Init(const WORLD_DESC& world_desc, const int& tileSize)
{
	m_desc = world_desc;

	InitPathmap(tileSize);
}

bool World::InitPathmap(const int& tileSize)
{
	std::ifstream myfile ("map.txt");
	if (!myfile.is_open())
	{
		return false;
	}
	std::string line;
	int lineIndex = 0;
	while (!myfile.eof())
	{
		std::getline (myfile,line);
		for (unsigned int i = 0; i < line.length(); i++)
		{
			Vector2f tilePos(i * tileSize, lineIndex * tileSize);
			if (line[i] == '.')
			{
				Dot* dot = new Dot(Vector2f(i * tileSize, lineIndex * tileSize));
				myDots.push_back(dot);
			}
			else if (line[i] == 'o')
			{
				BigDot* dot = new BigDot(tilePos);
				myBigDots.push_back(dot);
			}

			PathmapTile* tile = new PathmapTile(tilePos, (line[i] == 'x'));
			myPathmapTiles.push_back(tile);
		}

		++lineIndex;
	}

	myfile.close();
	return true;
}

void World::Draw(Renderer& renderer)
{
	renderer.DrawObject(*m_desc.playfieldDrawEntity);

	for (unsigned int i = 0; i < myDots.size(); ++i)
	{
		renderer.DrawObject(*m_desc.dotDrawEntity, myDots[i]->GetDrawPos().myX, myDots[i]->GetDrawPos().myY);
	}

	for (unsigned int i = 0; i < myBigDots.size(); ++i)
	{
		renderer.DrawObject(*m_desc.bigDotDrawEntity, myBigDots[i]->GetDrawPos().myX, myBigDots[i]->GetDrawPos().myY);
	}
}

bool World::TileIsValid(Vector2f tilePos)
{
	for (unsigned int i = 0; i < myPathmapTiles.size(); ++i)
	{
		if (myPathmapTiles[i]->m_pos == tilePos && !myPathmapTiles[i]->myIsBlockingFlag)
		{
			return true;
		}
	}
	return false;
}

bool World::HasIntersectedDot(GameEntity& gameEntity, bool& win)
{
	for (unsigned int i = 0; i < myDots.size(); ++i)
	{
		if (gameEntity.Intersect(myDots[i]->GetPosition(), m_dotIntersectionDist))
		{
			myDots.erase(myDots.begin() + i);
			if (myDots.size() == 0 && myBigDots.size() == 0)
			{
				win = true;
			}
			return true;
		}
	}
	return false;
}

bool World::HasIntersectedBigDot(GameEntity& gameEntity, bool& win)
{
	for (unsigned int i = 0; i < myBigDots.size(); ++i)
	{
		if (gameEntity.Intersect(myBigDots[i]->GetPosition(), m_dotIntersectionDist))
		{
			myBigDots.erase(myBigDots.begin() + i);
			if (myDots.size() == 0 && myBigDots.size() == 0)
			{
				win = true;
			}
			return true;
		}
	}
	return false;
}

bool World::HasIntersectedCherry(GameEntity& gameEntity)
{
	return true;
}

void World::Shutdown()
{
	SAFE_DELETE(m_desc.playfieldDrawEntity);
	SAFE_DELETE(m_desc.dotDrawEntity);
	SAFE_DELETE(m_desc.bigDotDrawEntity);
	SAFE_DELETE(m_desc.cherryDrawEntity);

	SAFE_DELETE_VECTOR(myPathmapTiles);
	SAFE_DELETE_VECTOR(myDots);
	SAFE_DELETE_VECTOR(myBigDots);
	SAFE_DELETE_VECTOR(myCherry);
}

PathmapTile* World::GetTile(Vector2f tilePos)
{
	for (unsigned int i = 0; i < myPathmapTiles.size(); ++i)
	{
		if (myPathmapTiles[i]->m_pos == tilePos)
		{
			return myPathmapTiles[i];
		}
	}
	return NULL;
}

bool World::ListDoesNotContain(const Vector2f& tilePos, int tileSize, std::list<PathmapTile*>& path)
{
	for (std::list<PathmapTile*>::iterator list_iter = path.begin(); list_iter != path.end(); list_iter++)
	{
		if (Pathfind((PathmapTile*)*list_iter, GetTile(tilePos), tileSize, path))
			return false;

		path.pop_back();
	}
	return true;
}

bool SortFromGhostSpawn(PathmapTile* a, PathmapTile* b)
{
	int la = abs(a->m_pos.myX - 13 * 22) + abs(a->m_pos.myY - 13 * 22);
	int lb = abs(b->m_pos.myX - 13 * 22) + abs(b->m_pos.myY - 13 * 22);

	return la < lb;
}

std::list<PathmapTile*> World::GetPath(Vector2f fromPos, Vector2f toPos, const int tileSize)
{
	for (unsigned int i = 0; i < myPathmapTiles.size(); ++i)
	{
		myPathmapTiles[i]->myIsVisitedFlag = false;
	}

	std::list<PathmapTile*> aList;

	Pathfind(GetTile(fromPos), GetTile(toPos), tileSize, aList);

	return aList;
}

bool World::Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, const int tileSize, std::list<PathmapTile*>& aList)
{
	aFromTile->myIsVisitedFlag = true;

	if (aFromTile->myIsBlockingFlag)
		return false;

	if (aFromTile == aToTile)
		return true;

	std::vector<PathmapTile*> neighborCollection;

	PathmapTile* upTile = GetTile(Vector2f(aFromTile->m_pos.myX, aFromTile->m_pos.myY - tileSize));
	PathmapTile* downTile = GetTile(Vector2f(aFromTile->m_pos.myX, aFromTile->m_pos.myY + tileSize));
	PathmapTile* leftTile = GetTile(Vector2f(aFromTile->m_pos.myX - tileSize, aFromTile->m_pos.myY));
	PathmapTile* rightTile = GetTile(Vector2f(aFromTile->m_pos.myX + tileSize, aFromTile->m_pos.myY));

	if (upTile && !upTile->myIsVisitedFlag && !upTile->myIsBlockingFlag && ListDoesNotContain(upTile->m_pos, tileSize, aList))
	{
		neighborCollection.push_back(upTile);
	}
	if (downTile && !downTile->myIsVisitedFlag && !downTile->myIsBlockingFlag && ListDoesNotContain(downTile->m_pos, tileSize, aList))
	{
		neighborCollection.push_back(downTile);
	}
	if (leftTile && !leftTile->myIsVisitedFlag && !leftTile->myIsBlockingFlag && ListDoesNotContain(leftTile->m_pos, tileSize, aList))
	{
		neighborCollection.push_back(leftTile);
	}
	if (rightTile && !rightTile->myIsVisitedFlag && !rightTile->myIsBlockingFlag && ListDoesNotContain(rightTile->m_pos, tileSize, aList))
	{
		neighborCollection.push_back(rightTile);
	}

	delete upTile;
	delete downTile;
	delete leftTile;
	delete rightTile;
	//neighborList.sort(SortFromGhostSpawn);

	for (unsigned int i = 0; i < neighborCollection.size(); ++i)
	{
		aList.push_back(neighborCollection[i]);

		if (Pathfind(neighborCollection[i], aToTile, tileSize, aList))
			return true;

		aList.pop_back();
	}

	return false;
}