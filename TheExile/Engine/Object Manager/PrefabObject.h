#ifndef _PREFAB_OBJECT_H_
#define _PREFAB_OBJECT_H_

#include "IObject.h"

class CObjectManager;

enum ETrap { ePressureSpike, eTimedSpike, eUnused, eSpinningBlade, eFireTrap, eMovingSpinBlade };
enum EObject { eBridge, eTree, eBush, eHealthChest, eLightWeaponChest, eHeavyWeaponChest, eDoor, eExitDoor, eOrcSpawn, eItemChest, eSHProp};


class CPrefabObject : public IObject
{

protected:

	
	vector<IObject*> m_vObjects;
	vector<CRenderMesh*> m_vRenderMeshes;

	vector<CMesh*> meshes;
	vector<string> diffuseTextures;
	vector<string> normalMaps;



	virtual void CreateSceneBounds();
	virtual void CreateRenderMeshes();
	virtual void AddColliders();

public:
	CPrefabObject();
	CPrefabObject(const char* filePath, const char* tag, CObjectManager* objManager = nullptr);
	~CPrefabObject();

	virtual void Update();

	void AddToRenderer();

	virtual void SetWorldMatrix(const XMFLOAT4X4 world);

	virtual bool LoadFromXML(const char* filePath, CObjectManager* objManager = nullptr);
	virtual void AddToObjectManager() final;
	virtual void AddChildrenToObjectManager() final;

	IObject* GetExitDoorObject();//only valid for exit zone prefabs

	vector<CRenderMesh*> GetPrefabRenderMeshes() { return m_vRenderMeshes; }

private:


};

#endif