#include "TileMap.h"

CTileMap::CTileMap(int2 MapSize)
{
	Resize(MapSize.x, MapSize.y);
}
CTileMap::~CTileMap()
{
	tiles.clear();
}
void CTileMap::Clear()
{
	for (int col = 0; col < m_tMapSize.x; col++)
	{
		for (int row = 0; row < m_tMapSize.y; row++)
			tiles[col * m_tMapSize.x + row] = CTile(col, row, CTile::WALL);
	}
}

CTile* CTileMap::GetTile(int col, int row)
{
	int index = col * m_tMapSize.x + row;
	if (index < (int)tiles.size() && index >= 0)
		return &tiles[index];
	return nullptr;
}
void CTileMap::Resize(int width, int height)
{
	m_tMapSize = int2(width, height);
	tiles.clear();
	tiles.resize(width * height);

	for (int col = 0; col < m_tMapSize.x; col++)
	{
		for (int row = 0; row < m_tMapSize.y; row++)
		{
			tiles[col * m_tMapSize.x + row] = CTile(col, row, CTile::WALL);

		}
	}
}
void CTileMap::GetTiles(CTile::ETileType type, vector<CTile*>& _tiles)
{
	for (int i = 0; i < (int)tiles.size(); i++)
	{
		if (tiles[i].TileType() == type)
			_tiles.push_back(&tiles[i]);
	}
}


