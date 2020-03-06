#include "World.h"

World::World(void)
{
}

World::~World(void)
{
}

void World::Init(Core& core, WORLD_DESC& world_desc)
{
	m_pPlayfield = new DrawEntity(core.GetRenderer(), world_desc.playfieldImage, 0, 0);
	m_pDot = new DrawEntity(core.GetRenderer(), world_desc.dotImage, 0, 0);
	m_pBigDot = new DrawEntity(core.GetRenderer(), world_desc.bigDotImage, 0, 0);

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

void World::Draw(Core& core)
{
	core.DrawObject(*m_pPlayfield);

	for (unsigned int i = 0; i < myDots.size(); ++i) 
	{
		core.DrawObject(*m_pDot, myDots[i]->GetDrawPos().myX, myDots[i]->GetDrawPos().myY);
	}

	for (unsigned int i = 0; i < myBigDots.size(); ++i)
	{
		core.DrawObject(*m_pBigDot, myBigDots[i]->GetDrawPos().myX, myBigDots[i]->GetDrawPos().myY);
	}
}

bool World::TileIsValid(int anX, int anY)
{
	for (unsigned int i = 0; i < myPathmapTiles.size(); ++i)
	{
		if (myPathmapTiles[i]->m_pos.myX == anX && myPathmapTiles[i]->m_pos.myY == anY && !myPathmapTiles[i]->myIsBlockingFlag)
		{
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedDot(const Vector2f& aPosition)
{
	for (unsigned int i = 0; i < myDots.size(); ++i)
	{
		if ((myDots[i]->GetPosition() - aPosition).Length() < 5.0f)
		{
			myDots.erase(myDots.begin() + i);
			return true;

			if (myDots.size() == 0 && myBigDots.size() == 0)
			{
				//You win
			}
		}
	}

	return false;
}

bool World::HasIntersectedBigDot(const Vector2f& aPosition)
{
	for (unsigned int i = 0; i < myBigDots.size(); ++i)
	{
		if ((myBigDots[i]->GetPosition() - aPosition).Length() < 5.0f)
		{
			myBigDots.erase(myBigDots.begin() + i);
			return true;

			if (myDots.size() == 0 && myBigDots.size() == 0)
			{
				//You win
			}
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
	delete m_pPlayfield;
	delete m_pDot;
	delete m_pBigDot;

	while (!myPathmapTiles.empty()) delete myPathmapTiles.back(), myPathmapTiles.pop_back();
	while (!myDots.empty()) delete myDots.back(), myDots.pop_back();
	while (!myBigDots.empty()) delete myBigDots.back(), myBigDots.pop_back();
	while (!myCherry.empty()) delete myCherry.back(), myCherry.pop_back();
}

void World::GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<PathmapTile*>& aList)
{
	PathmapTile* fromTile = GetTile(aFromX, aFromY);
	PathmapTile* toTile = GetTile(aToX, aToY);

	for (unsigned int i = 0; i < myPathmapTiles.size(); ++i)
	{
		myPathmapTiles[i]->myIsVisitedFlag = false;
	}

	Pathfind(fromTile, toTile, aList);
}

PathmapTile* World::GetTile(int aFromX, int aFromY)
{
	for (unsigned int i = 0; i < myPathmapTiles.size(); ++i)
	{
		if (myPathmapTiles[i]->m_pos.myX == aFromX && myPathmapTiles[i]->m_pos.myY == aFromY)
		{
			return myPathmapTiles[i];
		}
	}

	return NULL;
}

bool World::ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList)
{
	for(std::list<PathmapTile*>::iterator list_iter = aList.begin(); list_iter != aList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;
		if (tile == aFromTile)
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

bool World::Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList)
{
	aFromTile->myIsVisitedFlag = true;

	if (aFromTile->myIsBlockingFlag)
		return false;

	if (aFromTile == aToTile)
		return true;

	std::list<PathmapTile*> neighborList;

	PathmapTile* up = GetTile(aFromTile->m_pos.myX, aFromTile->m_pos.myY - 1);
	if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
		neighborList.push_front(up);

	PathmapTile* down = GetTile(aFromTile->m_pos.myX, aFromTile->m_pos.myY + 1);
	if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
		neighborList.push_front(down);

	PathmapTile* right = GetTile(aFromTile->m_pos.myX + 1, aFromTile->m_pos.myY);
	if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
		neighborList.push_front(right);

	PathmapTile* left = GetTile(aFromTile->m_pos.myX - 1, aFromTile->m_pos.myY);
	if (left && !left->myIsVisitedFlag && !left->myIsBlockingFlag && ListDoesNotContain(left, aList))
		neighborList.push_front(left);

	neighborList.sort(SortFromGhostSpawn);

	for(std::list<PathmapTile*>::iterator list_iter = neighborList.begin(); list_iter != neighborList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;

		aList.push_back(tile);

		if (Pathfind(tile, aToTile, aList))
			return true;

		aList.pop_back();
	}

	return false;
}