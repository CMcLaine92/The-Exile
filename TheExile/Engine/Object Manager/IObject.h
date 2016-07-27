/***********************************************
* Filename:  		IObject.h
* Date:      		8/11/2015
* Mod. Date: 		8/15/2015
* Mod. Initials:	NH
* Author:    		Spencer Slaton
* Purpose:			Base class for all objects
************************************************/

#ifndef IOBJECT_H
#define IOBJECT_H

#include "../Core/Collision/Collider.h"
#include "../Renderer/RenderMesh.h"
#include "../Renderer/Lights.h"
#include "../../Engine/Particles/ParticleManager.h"

class IObject
{

protected:

	struct Attachment
	{
		int type;

		union
		{
			IObject * obj;
			CRenderMesh * mesh;
		};

		string joint;
	};

	//Tag used for identification of objects
	std::string m_szTag;

	IObject* m_ipParent = nullptr;

	std::vector<IObject*> m_pvChildren;

	vector<Attachment> m_vpAttachments;

	std::vector<CCollider *> m_pvColliders;

	DirectX::XMFLOAT4X4 m_mWorld; //initialized to identity

	CRenderMesh* m_cpRenderMesh = nullptr;

	XMFLOAT3 m_f3Velocity = {0.0f,0.0f,0.0f};

	bool m_bFrustumCull = true;

	vector<TPointLight> m_vtPointLights;

	//XMFLOAT3 m_f3WorldVelocity = { 0.0f, 0.0f, 0.0f };
public:

	IObject();

	IObject(std::string _szTag);
	virtual ~IObject();

	virtual void Update() = 0;

	void UpdateAttachments();

	virtual void UpdatePosition();

	virtual XMFLOAT3 GetWorldVelocity();

	virtual void SetWorldVelocity(XMFLOAT3 f3Velocity);
	
	virtual void UpdateColliders(XMFLOAT3 f3Velocity);

	virtual void CollisionResponse(IObject * _cCollidedObject);

	void AddCollider(CCollider* collider);
	void RemoveCollider(CCollider* collider);


	void AttachToJoint(CRenderMesh * _mesh, string _joint);

	void AttachToJoint(IObject * _object, string _joint);
	void DetachObject(IObject * _object);

	void LookAt(XMFLOAT3 target);

	virtual void AddToRenderer();

	/***************
	*  Accessors
	***************/

	std::string& GetTag()							{ return m_szTag; }
	std::vector<CCollider *>& GetColliders()		{ return m_pvColliders; };
	const DirectX::XMFLOAT3 * GetPosition() const	{ return reinterpret_cast<const DirectX::XMFLOAT3 *>(&m_mWorld._41); }
	const DirectX::XMFLOAT3 * GetYAxis() const		{ return reinterpret_cast<const DirectX::XMFLOAT3 *>(&m_mWorld._21); }
	const DirectX::XMFLOAT3 * GetZAxis() const		{ return reinterpret_cast<const DirectX::XMFLOAT3 *>(&m_mWorld._31); }
	XMFLOAT4X4 GetWorldMatrix()						{ return m_mWorld; }
	CRenderMesh* GetRenderMesh() 					{ return m_cpRenderMesh; }
	XMFLOAT3 GetVelocity()							{ return m_f3Velocity; }
	IObject * GetParent()							{ return m_ipParent; }
	bool GetFrustumCull()							{ return m_bFrustumCull; }
	vector<TPointLight> GetPointLights()			{ return m_vtPointLights; }

	/***************
	* Mutators
	***************/

	virtual void SetTag(const char* _tag){ m_szTag = _tag; }

	virtual void SetWorldMatrix(const DirectX::XMFLOAT4X4 matrix);
	void SetPosition(const DirectX::XMFLOAT3 pos);
	void SetXAxis(const DirectX::XMFLOAT3 xAxis);
	void SetYAxis(const DirectX::XMFLOAT3 yAxis);
	void SetZAxis(const DirectX::XMFLOAT3 zAxis);
	void Scale(float scaleX, float scaleY, float scaleZ);
	void SetRenderMesh(CRenderMesh* renderMesh);
	void SetParent(IObject * _cParent) { m_ipParent = _cParent; }
	void AddChild(IObject * _cChild) { m_pvChildren.push_back(_cChild); }
	void SetVelocity(XMFLOAT3 f3Velocity) { m_f3Velocity = f3Velocity; }
	void SetFrustumCull(bool _cull) {m_bFrustumCull = _cull;}
	void AddLight(TPointLight light) { m_vtPointLights.push_back(light); }
};

#endif
