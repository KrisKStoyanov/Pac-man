#include "World.h"

World::World(void) : 
	m_desc(),
	m_pCherry(nullptr),
	m_pathmapTileCollection(),
	m_dotCollection(),
	m_bigDotCollection(),
	m_dotIntersectionDist(5.0f),
	m_spawnCherryDefault(30.0f),
	m_spawnCherryDuration(30.0f),
	m_spawnCherryReducer(1.0f)
{

}

World::~World(void)
{
}

void World::Init(const WORLD_DESC& world_desc)
{
	m_desc = world_desc;

	InitPathmap(m_desc.tileSize);
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
				m_dotCollection.push_back(dot);
			}
			else if (line[i] == 'o')
			{
				BigDot* dot = new BigDot(tilePos);
				m_bigDotCollection.push_back(dot);
			}

			PathmapTile* tile = new PathmapTile(tilePos, (line[i] == 'x'));
			m_pathmapTileCollection.push_back(tile);
		}

		++lineIndex;
	}

	myfile.close();
	return true;
}

void World::Update(const float& deltaTime)
{
	if (!m_pCherry)
	{
		m_spawnCherryDuration -= deltaTime * m_spawnCherryReducer;
		if (m_spawnCherryDuration < 0.0f)
		{
			SpawnCherry();
			m_spawnCherryDuration = m_spawnCherryDefault;
		}
	}
}

void World::Reset()
{
	m_spawnCherryDuration = m_spawnCherryDefault;
	SAFE_DELETE(m_pCherry);
}

void World::Draw(Renderer& renderer)
{
	renderer.DrawObject(*m_desc.playfieldDrawEntity);

	for (unsigned int i = 0; i < m_dotCollection.size(); ++i)
	{
		renderer.DrawObject(*m_desc.dotDrawEntity, m_dotCollection[i]->GetDrawPos().myX, m_dotCollection[i]->GetDrawPos().myY);
	}

	for (unsigned int i = 0; i < m_bigDotCollection.size(); ++i)
	{
		renderer.DrawObject(*m_desc.bigDotDrawEntity, m_bigDotCollection[i]->GetDrawPos().myX, m_bigDotCollection[i]->GetDrawPos().myY);
	}

	if (m_pCherry)
	{
		renderer.DrawObject(*m_desc.cherryDrawEntity, m_pCherry->GetDrawPos().myX, m_pCherry->GetDrawPos().myY);
	}
}

bool World::TileIsValid(Vector2f tilePos)
{
	for (unsigned int i = 0; i < m_pathmapTileCollection.size(); ++i)
	{
		if (m_pathmapTileCollection[i]->m_pos == tilePos && !m_pathmapTileCollection[i]->myIsBlockingFlag)
		{
			return true;
		}
	}
	return false;
}

bool World::HasIntersectedDot(GameEntity& gameEntity, bool& win)
{
	for (unsigned int i = 0; i < m_dotCollection.size(); ++i)
	{
		if (gameEntity.Intersect(m_dotCollection[i]->GetPosition(), m_dotIntersectionDist))
		{
			m_dotCollection.erase(m_dotCollection.begin() + i);
			if (m_dotCollection.size() == 0 && m_bigDotCollection.size() == 0)
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
	for (unsigned int i = 0; i < m_bigDotCollection.size(); ++i)
	{
		if (gameEntity.Intersect(m_bigDotCollection[i]->GetPosition(), m_dotIntersectionDist))
		{
			m_bigDotCollection.erase(m_bigDotCollection.begin() + i);
			if (m_dotCollection.size() == 0 && m_bigDotCollection.size() == 0)
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
	if (m_pCherry)
	{
		if (gameEntity.Intersect(m_pCherry->GetPosition(), m_dotIntersectionDist))
		{
			SAFE_DELETE(m_pCherry);
			return true;
		}
		return false;
	}
	return false;
}

void World::Shutdown()
{
	SAFE_DELETE(m_desc.playfieldDrawEntity);
	SAFE_DELETE(m_desc.dotDrawEntity);
	SAFE_DELETE(m_desc.bigDotDrawEntity);
	SAFE_DELETE(m_desc.cherryDrawEntity);

	SAFE_DELETE_VECTOR(m_pathmapTileCollection);
	SAFE_DELETE_VECTOR(m_dotCollection);
	SAFE_DELETE_VECTOR(m_bigDotCollection);
	SAFE_DELETE(m_pCherry);
}

void World::SpawnCherry(const Vector2f& position)
{
	m_pCherry = new Cherry(position * m_desc.tileSize);
}

PathmapTile* World::GetTile(Vector2f tilePos)
{
	for (unsigned int i = 0; i < m_pathmapTileCollection.size(); ++i)
	{
		if (m_pathmapTileCollection[i]->m_pos == tilePos)
		{
			return m_pathmapTileCollection[i];
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

bool World::SortFromGhostSpawn(PathmapTile* a, PathmapTile* b)
{
	int la = 
		abs(a->m_pos.myX - 13 * m_desc.tileSize) + 
		abs(a->m_pos.myY - 13 * m_desc.tileSize);

	int lb = 
		abs(b->m_pos.myX - 13 * m_desc.tileSize) + 
		abs(b->m_pos.myY - 13 * m_desc.tileSize);

	return la < lb;
}

std::list<PathmapTile*> World::GetPath(Vector2f fromPos, Vector2f toPos, const int tileSize)
{
	for (unsigned int i = 0; i < m_pathmapTileCollection.size(); ++i)
	{
		m_pathmapTileCollection[i]->myIsVisitedFlag = false;
	}

	std::list<PathmapTile*> aList;

	Pathfind(GetTile(fromPos), GetTile(toPos), tileSize, aList);

	return aList;
}

float World::GetRandFloat(float seed)
{
	//double x = rand() / static_cast<double>(RAND_MAX + 1);

	//// [0,1[ * (max - min) + min is in [min,max[
	//int that = min + static_cast<int>(x * (max - min));

	//3.759816f

	//float r = 3.83f - seed;
	//float x = seed;
	//float biforcation = r * x * (1 - x);

	//float s = sin(seed + biforcation);
	return (rand() % 10) + 1;
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