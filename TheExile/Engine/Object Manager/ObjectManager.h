#pragma once
#include "QuadTree.h"
#include "../Core/Collision/Collision.h"
class IObject;

class CObjectManager
{
public:
	enum eObjectType {Static, Dynamic, AI, Count};

private:
	CQuadTree * m_cQuadTree = nullptr;
	std::vector<IObject *> m_vpObjects[Count];
	TFrustum* m_ptFrustum = nullptr;
public:

	void AddObject(IObject * _iObject, eObjectType _eType);
	bool RemoveObject(IObject * _iObject, eObjectType _eType);
	vector<IObject *> FindObjectsWithTag(string _szTag, eObjectType _eType);
	IObject * GetCamera();
	IObject * GetPlayer();
	TFrustum* GetFrustum() { return m_ptFrustum; }
	void UpdateAll();
	std::vector<IObject *>& GetList(eObjectType _type) { return m_vpObjects[_type]; }


	CQuadTree* GetQuadTree() { return m_cQuadTree; }
	CObjectManager();

	~CObjectManager();
};

