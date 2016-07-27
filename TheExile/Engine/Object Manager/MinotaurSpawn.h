#ifndef _MINOTAUR_SPAWN_H
#define _MINOTAUR_SPAWN_H

#include "PrefabObject.h"

#define MAX_OPEN_TIME 1.0f
#define RIGHT_MESH_INDEX 4
#define LEFT_MESH_INDEX 5

#define RIGHT_COLLIDER_INDEX 12
#define LEFT_COLLIDER_INDEX 13

enum EDirection{eDefault = -1, eNorth, eSouth, eEast, eWest};
class CDayNight;
class CMinotaurSpawn : public CPrefabObject
{
public:
	CMinotaurSpawn();
	CMinotaurSpawn(const char* filePath, CObjectManager* objManager, CDayNight* dayNight);
	~CMinotaurSpawn();
	virtual void SetWorldMatrix(const XMFLOAT4X4 world);
	virtual void Update();

private:
	virtual void AddColliders();
	virtual bool LoadFromXML(const char* prefabName, CObjectManager* objManager);

	void FindDoorStartAndEnd(int DoorMeshIndex, XMFLOAT3 position);
	XMFLOAT3 LerpPosition(XMFLOAT3 start, XMFLOAT3 end, float lamda);

	vector<XMFLOAT4X4> meshLocalMatricies;
	XMFLOAT3 startLeftPos;
	XMFLOAT3 endLeftPos;
	XMFLOAT3 startRightPos;
	XMFLOAT3 endRightPos;

	const float startPositionLeft = -150.0f;
	const float endPositionLeft = -350.0f;

	const float startPositionRight = 150.0f;
	const float endPositionRight = 350.0f;


	CDayNight* m_pDayNight;
	float LerpRatio;
	bool isMoving;
	bool isDay;
//	float m_fMoveTime;
//	bool m_bIsDay;
	EDirection m_eDirection;
};












#endif