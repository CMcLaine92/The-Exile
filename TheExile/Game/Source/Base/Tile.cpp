#include "Tile.h"


CTile::CTile(int x, int z, ETileType _type)
{
	m_tPosition = XMFLOAT3((float)x, 0.0f, (float)z);
	Make(_type);
	m_eNeighborType = UNKNOWN;
}
CTile::CTile()
{
	m_tPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Make(WALL);
	 m_eNeighborType = UNKNOWN;
}
void CTile::Make(ETileType _type)
{
	m_eTileType = _type;
	switch (m_eTileType)
	{
	case CRACKED_WALL: // GRAY
		m_tUVs = XMFLOAT4(50, 10, 60, 20);
		break;
	case WALL: // GRAY
		m_tUVs = XMFLOAT4(10, 10, 20, 20);
		break;
	case FLOOR: //WHITE
		m_tUVs = XMFLOAT4(30, 0, 40, 10);
		break;
	case ROOM_FLOOR:
		m_tUVs = XMFLOAT4(30, 0, 40, 10);
		break;
	case GREEN_ZONE: //LIGHT GREEN
	case SAFE_ZONE://LIGHT GREEN
		m_tUVs = XMFLOAT4(0, 0, 10, 10);
		break;
	case EXIT: //RED
		m_tUVs = XMFLOAT4(10, 0, 20, 10);
		break;
	case SECRET_ROOM: //BLUE
		m_tUVs = XMFLOAT4(20, 0, 30, 10);
		break;
	case TORCH:
	case CHEST: //YELLOW
		m_tUVs = XMFLOAT4(0, 10, 10, 20);
		break;
	case BUSHES: //DARK GREEN
		m_tUVs = XMFLOAT4(20, 10, 30, 20);
		break;
	case TREES: //BROWNISH
		m_tUVs = XMFLOAT4(30, 10, 40, 20);
		break;
	case PIT: //BLACK
		m_tUVs = XMFLOAT4(40, 0, 50, 10);
		break;
	case MAZE_START:
		m_tUVs = XMFLOAT4(40, 10, 50, 20);
		break;
	case BORDER:
		m_tUVs = XMFLOAT4(50, 0, 60, 10);
		break;
	case MIN_SPAWN:
		m_tUVs = XMFLOAT4(0, 20, 10, 30);
		break;
	case SKEL_SPAWN:
		m_tUVs = XMFLOAT4(10, 20, 20, 30);
		break;
	case ORC_SPAWN:
		m_tUVs = XMFLOAT4(20, 20, 30, 30);
		break;
	default:
		break;
	}
}
