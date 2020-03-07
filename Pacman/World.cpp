#include "World.h"

World::World(void)
{
}

World::~World(void)
{
}

void World::Init(const WORLD_DESC& world_desc)
{
	//fix these pointers (copy constructor/move assignment)
	//m_pPlayfield = world_desc.playfieldDrawEntity; 
	//m_pDot = world_desc.dotDrawEntity;
	//m_pBigDot = world_desc.bigDotDrawEntity;

	m_desc = world_desc;

	InitPathmap();
}

bool World::InitPathmap()
{
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] == '.')
				{
					Dot* dot = new Dot(Vector2f(i * 22, lineIndex * 22));
					myDots.push_back(dot);
				}
				else if (line[i] == 'o')
				{
					BigDot* dot = new BigDot(Vector2f(i * 22, lineIndex * 22));
					myBigDots.push_back(dot);
				}

				PathmapTile* tile = new PathmapTile(i, lineIndex, (line[i] == 'x'));
				myPathmapTiles.push_back(tile);
			}

			lineIndex++;
		}
		myfile.close();
	}

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

bool World::HasIntersectedDot(const Vector2f& aPosition, bool& win)
{
	for (unsigned int i = 0; i < myDots.size(); ++i)
	{
		if ((myDots[i]->GetPosition() - aPosition).Length() < 5.0f)
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

bool World::HasIntersectedBigDot(const Vector2f& aPosition, bool& win)
{
	for (unsigned int i = 0; i < myBigDots.size(); ++i)
	{
		if ((myBigDots[i]->GetPosition() - aPosition).Length() < 5.0f)
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

bool World::HasIntersectedCherry(const Vector2f& aPosition)
{
	return true;
}

void World::Shutdown()
{
	delete m_desc.playfieldDrawEntity;
	delete m_desc.dotDrawEntity;
	delete m_desc.bigDotDrawEntity;
	delete m_desc.cherryDrawEntity;

	while (!myPathmapTiles.empty()) delete myPathmapTiles.back(), myPathmapTiles.pop_back();
	while (!myDots.empty()) delete myDots.back(), myDots.pop_back();
	while (!myBigDots.empty()) delete myBigDots.back(), myBigDots.pop_back();
	while (!myCherry.empty()) delete myCherry.back(), myCherry.pop_back();
}

std::vector<PathmapTile*> World::GetPath(Vector2f fromPos, Vector2f toPos)
{
	PathmapTile* fromTile = GetTile(fromPos);
	PathmapTile* toTile = GetTile(toPos);

	for (unsigned int i = 0; i < myPathmapTiles.size(); ++i)
	{
		myPathmapTiles[i]->myIsVisitedFlag = false;
	}

	std::vector<PathmapTile*> aList;

	Pathfind(fromTile, toTile, aList);

	return aList;
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

bool World::ListDoesNotContain(PathmapTile* aFromTile, std::vector<PathmapTile*>& aList)
{
	for (unsigned int i = 0; i < aList.size(); ++i) 
	{
		if (aList[i] == aFromTile)
		{
			return false;
		}
	}

	return true;
}

bool SortFromGhostSpawn(PathmapTile* a, PathmapTile* b)
{
	int la = abs(a->m_pos.myX - 13) + abs(a->m_pos.myY - 13);
	int lb = abs(b->m_pos.myX - 13) + abs(b->m_pos.myY - 13);

    return la < lb;
}

bool World::Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::vector<PathmapTile*>& aList)
{
	aFromTile->myIsVisitedFlag = true;

	if (aFromTile->myIsBlockingFlag)
		return false;

	if (aFromTile == aToTile)
		return true;

	std::list<PathmapTile*> neighborList;

	PathmapTile* up = GetTile(Vector2f(aFromTile->m_pos.myX, aFromTile->m_pos.myY - 1));
	if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
		neighborList.push_front(up);

	PathmapTile* down = GetTile(Vector2f(aFromTile->m_pos.myX, aFromTile->m_pos.myY + 1));
	if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
		neighborList.push_front(down);

	PathmapTile* right = GetTile(Vector2f(aFromTile->m_pos.myX + 1, aFromTile->m_pos.myY));
	if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
		neighborList.push_front(right);

	PathmapTile* left = GetTile(Vector2f(aFromTile->m_pos.myX - 1, aFromTile->m_pos.myY));
	if (left && !left->myIsVisitedFlag && !left->myIsBlockingFlag && ListDoesNotContain(left, aList))
		neighborList.push_front(left);

	neighborList.sort(SortFromGhostSpawn);

	for(std::list<PathmapTile*>::iterator list_iter = neighborList.begin(); list_iter != neighborList.end(); list_iter++)
	{
		aList.push_back((PathmapTile*)*list_iter);

		if (Pathfind((PathmapTile*)*list_iter, aToTile, aList))
			return true;

		aList.pop_back();
	}

	return false;
}