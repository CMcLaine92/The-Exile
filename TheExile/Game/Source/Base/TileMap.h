#ifndef _TILE_MAP_H_
#define _TILE_MAP_H_
#include "Tile.h"


class CTileMap
{
public:
	CTileMap(int2 MapSize);
	~CTileMap();

	CTile* GetTile(int col, int row);
	void Resize(int width, int height);
	void Clear();
	void GetTiles(CTile::ETileType type, vector<CTile*>& _tiles);
	void SetTileBlock(CTile::ETileType _type, XMFLOAT3 _pos, XMFLOAT2 _size);

	int2& Size(){ return m_tMapSize; }
	const int2& Size() const { return m_tMapSize; }
	vector<CTile>& GetAllTiles() { return tiles; }


private:

	vector<CTile> tiles;
	int2 m_tMapSize;

};


#endif
