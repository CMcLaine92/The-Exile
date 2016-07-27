
#ifndef CWORLDMAP_H
#define CWORLDMAP_H

#include <unordered_map>

class CHUDMesh;
class IObject;
class CTileMap;

struct TFog
{
	CHUDMesh* pcMesh = nullptr;
	bool bIsActive = true;
	float fLeft = 0.0f;
	float fRight = 0.0f;
	float fTop = 0.0f;
	float fBottom = 0.0f;

};

class CWorldMap
{
private:

	CHUDMesh* m_pcMap = nullptr;
	CHUDMesh* m_pcOverlay = nullptr;
	CHUDMesh* m_pcPlayerIndicator = nullptr;
	CHUDMesh* m_pcTotalMap = nullptr;

	//std::vector<CHUDMesh*> m_vpcTiles;
	IObject* m_pcPlayer = nullptr;
	bool m_bIsOpen = false;
	vector<TFog> m_vFog;

	vector<CHUDMesh*> m_vpOldTombs;
	unordered_map<unsigned int, CHUDMesh*> m_umTombstoneIcons;
	bool m_bTomstoneActive = false;//true if one tombstone is active

	vector<CHUDMesh*> m_vpDestroyedWalls;

	bool m_bKeyFound;
	bool m_bCollidedBridge;
	bool m_bBridgeBuilt;

	void CreateFog();

public:

	void Initialize(CTileMap* pcTileMap);

	CWorldMap(IObject* pcPlayer);
	~CWorldMap();

	void Update();

	void Open();
	void Close();
	bool IsOpen(){ return m_bIsOpen; }

	vector<TFog>& GetFog() {return m_vFog;}

	void AddTombstoneIcon(unsigned int ID);
	void RemoveTombstoneIcon(unsigned int ID);
	void DeActivateTombstones();

	void AddDestroyedWallIcon(const XMFLOAT3* pos);

	void AddToRenderer();

	void ExitKeyFound();
	void BridgeCollided();
	void BridgeBuilt();

};

#endif
