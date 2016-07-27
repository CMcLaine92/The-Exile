#ifndef _TILE_H_
#define _TILE_H_


	


class CTile
{
public:
	enum ETileType { WALL, CRACKED_WALL, GREEN_WALL, FLOOR, SAFE_ZONE, EXIT, GREEN_ZONE, SECRET_ROOM, CHEST, BUSHES, TREES, PIT, MAZE_START, BORDER, MIN_SPAWN, SKEL_SPAWN, ORC_SPAWN, ROOM_FLOOR, TORCH, TRAP };
	enum ENeighborType { LEFT, TOP, RIGHT, BOTTOM, UNKNOWN };

	

	CTile(int x, int z, ETileType _type);
	CTile();

	//Accessors
	const XMFLOAT4& GetUVS() const { return m_tUVs; }
	const XMFLOAT3& GetPosition() const { return m_tPosition; }
	const ETileType& TileType() const { return m_eTileType; }
	const ENeighborType& NeighborType() const{ return m_eNeighborType; }



	//Mutators
	XMFLOAT4& GetUVS()  { return m_tUVs; }
	XMFLOAT3& GetPosition()  { return m_tPosition; }
	ETileType& TileType()  { return m_eTileType; }
	ENeighborType& NeighborType() { return m_eNeighborType; }

	void Make(ETileType _type);


private:

	XMFLOAT4 m_tUVs;
	XMFLOAT3 m_tPosition;
	ETileType m_eTileType;
	ENeighborType m_eNeighborType;



};





#endif