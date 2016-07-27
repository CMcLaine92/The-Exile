/***********************************************
* Filename:  		IObject.cpp
* Date:      		8/11/2015
* Mod. Date: 		8/11/2015, 8/21/2015
* Mod. Initials:	SS, MZ
* Author:    		Spencer Slaton
* Purpose:			Base class for all objects
************************************************/

#include "IObject.h"
#include "../Animation/AnimationMesh.h"
#include <typeindex>
#include "../../Engine/Renderer/Renderer.h"
#include "../../Engine/Particles/Emitter.h"
#include "../../Engine/Particles/Particle.h"

#if _DEBUG 
#include "../Core/Collision/AABB.h"

#endif
/*****************************************************************
* IObject():		Initializes the Object
*
* Ins:				string
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
IObject::IObject(std::string _szTag)
{
	m_szTag = _szTag;
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
}
IObject::IObject()
{
	m_szTag = "Default";
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
}

/*****************************************************************
* ~IObject():		Terminates the Object
*
* Ins:				N/A
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
IObject::~IObject()
{
	SAFE_DELETE(m_cpRenderMesh);

	VECTOR_DELETE_ALL(m_pvColliders);
}

/*****************************************************************
* AddCollider():	Adds a collider to this object
*
* Ins:				CCollider*
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/15/2015
* Mod. Initials:	NH
*****************************************************************/
void IObject::AddCollider(CCollider* collider)
{
	if (collider != nullptr)
	{
		m_pvColliders.push_back(collider);
	}
}

/*****************************************************************
* AddCollider():	Adds a collider to this object
*
* Ins:				CCollider*
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/15/2015
* Mod. Initials:	NH
*****************************************************************/
void IObject::SetWorldMatrix(const DirectX::XMFLOAT4X4 matrix)
{
	m_mWorld = matrix;
}

/*****************************************************************
* SetPosition():	Sets the objects position in it's world matrix
*
* Ins:				XMFLOAT3
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
void IObject::SetPosition(const XMFLOAT3 pos)
{
	m_mWorld._41 = pos.x;
	m_mWorld._42 = pos.y;
	m_mWorld._43 = pos.z;
}

/*****************************************************************
* SetXAxis():		Sets the object's world matrix's X Axis
*
* Ins:				XMFLOAT3
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
void IObject::SetXAxis(const XMFLOAT3 xAxis)
{
	m_mWorld._11 = xAxis.x;
	m_mWorld._12 = xAxis.y;
	m_mWorld._13 = xAxis.z;
}

/*****************************************************************
* SetYAxis():		Sets the object's world matrix's Y Axis
*
* Ins:				XMFLOAT3
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
void IObject::SetYAxis(const XMFLOAT3 yAxis)
{
	m_mWorld._21 = yAxis.x;
	m_mWorld._22 = yAxis.y;
	m_mWorld._23 = yAxis.z;
}

/*****************************************************************
* SetZAxis():		Sets the object's world matrix's Z Axis
*
* Ins:				XMFLOAT3
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/
void IObject::SetZAxis(const XMFLOAT3 zAxis)
{
	m_mWorld._31 = zAxis.x;
	m_mWorld._32 = zAxis.y;
	m_mWorld._33 = zAxis.z;
}

void  IObject::Scale(float scaleX, float scaleY, float scaleZ)
{

	XMVECTOR quat = XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_mWorld));
	XMFLOAT3 pos = *GetPosition();
	XMMATRIX newScale = XMMatrixIdentity();
	XMMATRIX newMat = XMMatrixIdentity();
	
	newScale = XMMatrixScaling(scaleX, scaleY, scaleZ);

	newMat = newScale * XMMatrixRotationQuaternion(quat);
	XMStoreFloat4x4(&m_mWorld, newMat);
	m_mWorld._41 = pos.x;
	m_mWorld._42 = pos.y;
	m_mWorld._43 = pos.z;

	if (m_cpRenderMesh)
		m_cpRenderMesh->GetPositionMatrix() = m_mWorld;
}

/*****************************************************************
* AddCollider():	Adds a collider to this object
*
* Ins:				CCollider*
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/15/2015
* Mod. Initials:	NH
*****************************************************************/
void IObject::SetRenderMesh(CRenderMesh* renderMesh)
{
	m_cpRenderMesh = renderMesh;
}

/*****************************************************************
* CollisionResponse():	Called after a collision is made between this object and another
*
* Ins:					N/A
*
* Outs:					N/A
*
* Returns:				N/A
*
* Mod. Date:			8/15/2015
* Mod. Initials:		NH
*****************************************************************/
void IObject::CollisionResponse(IObject * _cCollidedObject)
{
	DBG_UNREFERENCED_PARAMETER(_cCollidedObject);
	//handles collision response for this object
}

/*****************************************************************
* UpdatePosition():		Updates position and colliders based on Velocity
*
* Ins:					N/A
*
* Outs:					N/A
*
* Returns:				N/A
*
* Mod. Date:			8/20/2015
* Mod. Initials:		MZ
*****************************************************************/
void IObject::UpdatePosition()
{
	XMFLOAT4X4 oldWorldMAtrix = m_mWorld;

	XMMATRIX d3dTranslateZ = XMMatrixTranslation(m_f3Velocity.x * DELTA_TIME(), m_f3Velocity.y * DELTA_TIME(), m_f3Velocity.z * DELTA_TIME());
	XMStoreFloat4x4(&m_mWorld, XMMatrixMultiply(d3dTranslateZ, XMLoadFloat4x4(&m_mWorld)));

	if (m_cpRenderMesh != nullptr)
	{
		m_cpRenderMesh->GetPositionMatrix() = m_mWorld;
	}


	XMFLOAT3 f3VelocityUpdate = GetWorldVelocity();
	f3VelocityUpdate.x *= DELTA_TIME();
	f3VelocityUpdate.y *= DELTA_TIME();
	f3VelocityUpdate.z *= DELTA_TIME();

	//if (f3VelocityUpdate.y < -30.0f)
	//{
	//	f3VelocityUpdate.y = -30.0f;
	//}


	for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
	{
		XMFLOAT3 fResetVel = m_pvColliders[collider]->GetResetVelocity();
		fResetVel.x *= DELTA_TIME();
		fResetVel.y *= DELTA_TIME();
		fResetVel.z *= DELTA_TIME();

		m_pvColliders[collider]->UpdatePosition(fResetVel);

		m_pvColliders[collider]->UpdatePosition(f3VelocityUpdate);
	}

	UpdateAttachments();
}

void IObject::UpdateAttachments()
{
	for (unsigned int i = 0; i < m_vpAttachments.size(); i++)
	{
		XMFLOAT4X4 joint = ((CAnimationMesh*)m_cpRenderMesh)->GetJointMatrix(m_vpAttachments[i].joint);

		if (m_vpAttachments[i].type == 0)
			XMStoreFloat4x4(&m_vpAttachments[i].mesh->GetPositionMatrix(), XMMatrixMultiply(XMLoadFloat4x4(&joint), XMLoadFloat4x4(&m_mWorld)));
		else
			XMStoreFloat4x4(&m_vpAttachments[i].obj->GetWorldMatrix(), XMMatrixMultiply(XMLoadFloat4x4(&joint), XMLoadFloat4x4(&m_mWorld)));
	}
}

void IObject::DetachObject(IObject * _object)
{
	for (unsigned int i = 0; i < m_vpAttachments.size(); i++)
	{
		if (m_vpAttachments[i].obj == _object)
		{
			m_vpAttachments.erase(m_vpAttachments.begin() + i);
			return;
		}
	}
}




/*****************************************************************
* UpdateColliders():	Updates colliders based on Velocity
*
* Ins:					XMFLOAT3 f3Velocity
*
* Outs:					N/A
*
* Returns:				N/A
*
* Mod. Date:			8/20/2015
* Mod. Initials:		MZ
*****************************************************************/
void IObject::UpdateColliders(XMFLOAT3 f3Velocity)
{
	f3Velocity.x *= DELTA_TIME();
	f3Velocity.y *= DELTA_TIME();
	f3Velocity.z *= DELTA_TIME();

	for (unsigned int collider = 0; collider < m_pvColliders.size(); collider++)
	{
		m_pvColliders[collider]->UpdatePosition(f3Velocity);
	}
#if _DEBUG
	DEBUG_UTIL::DrawAABBCollider(((CAABB*)m_pvColliders[0]->GetBounds())->Min(), ((CAABB*)m_pvColliders[0]->GetBounds())->Max());
#endif
}

/*****************************************************************
* GetWorldVelocty():	Returns World Velocity
*
* Ins:					N/A
*
* Outs:					XMFLOAT3 - World Velocity
*
* Returns:				N/A
*
* Mod. Date:			8/20/2015
* Mod. Initials:		MZ
*****************************************************************/
XMFLOAT3 IObject::GetWorldVelocity()
{
	XMFLOAT4X4 oldWorldMAtrix = m_mWorld;

	XMMATRIX d3dTranslateZ = XMMatrixTranslation(m_f3Velocity.x, m_f3Velocity.y, m_f3Velocity.z);

	XMFLOAT4X4 newWorld;
	XMStoreFloat4x4(&newWorld, XMMatrixMultiply(d3dTranslateZ, XMLoadFloat4x4(&m_mWorld)));

	XMFLOAT3 f3VelocityUpdate;
	f3VelocityUpdate.x = newWorld._41 - oldWorldMAtrix._41;
	f3VelocityUpdate.y = newWorld._42 - oldWorldMAtrix._42;
	f3VelocityUpdate.z = newWorld._43 - oldWorldMAtrix._43;

	return f3VelocityUpdate;
}

/*****************************************************************
* SetWorldVelocty():	Sets Local Velocity based on passed in World Velocity and
*						current world matrix
*
* Ins:					XMFLOAT3 - World Velocity
*
* Outs:					N/A
*
* Returns:				N/A
*
* Mod. Date:			8/20/2015
* Mod. Initials:		MZ
*****************************************************************/
void IObject::SetWorldVelocity(XMFLOAT3 f3Velocity)
{
	XMMATRIX matTranslateZ = XMMatrixTranslation(f3Velocity.x, f3Velocity.y, f3Velocity.z);
	XMFLOAT4X4 matLocalSpace;
	XMMATRIX matWorld = XMLoadFloat4x4(&m_mWorld);
	matWorld.r[0].m128_f32[3] = 0.0f;
	matWorld.r[1].m128_f32[3] = 0.0f;
	matWorld.r[2].m128_f32[3] = 0.0f;

	matWorld.r[3].m128_f32[0] = 0.0f;
	matWorld.r[3].m128_f32[1] = 0.0f;
	matWorld.r[3].m128_f32[2] = 0.0f;

	matWorld = XMMatrixInverse(&XMMatrixDeterminant(matWorld), matWorld);
	XMStoreFloat4x4(&matLocalSpace, XMMatrixMultiply(matTranslateZ, matWorld));
	XMFLOAT3 f3LocalVelocity;
	f3LocalVelocity.x = matLocalSpace._41;
	f3LocalVelocity.y = matLocalSpace._42;
	f3LocalVelocity.z = matLocalSpace._43;

	SetVelocity(f3LocalVelocity);
}

void IObject::AttachToJoint(CRenderMesh * _mesh, string _joint)
{
	if (m_cpRenderMesh->GetType() == typeid(CAnimationMesh))
	{
		Attachment temp;
		temp.type = 0;
		temp.mesh = _mesh;
		temp.joint = _joint;
		m_vpAttachments.push_back(temp);
	}
}

void IObject::AttachToJoint(IObject * _object, string _joint)
{
	if (m_cpRenderMesh->GetType() == typeid(CAnimationMesh))
	{
		Attachment temp;
		temp.type = 0;
		temp.obj = _object;
		temp.joint = _joint;
		m_vpAttachments.push_back(temp);
	}
}

void IObject::LookAt(XMFLOAT3 target)
{
	XMVECTOR newX, newY, newZ, globalY;
	XMVECTOR targetLocation;
	XMFLOAT3 xAxis, yAxis, zAxis;
	XMFLOAT3 up = XMFLOAT3(0, 1, 0);
	XMVECTOR currentLocation = XMLoadFloat3(GetPosition());

	globalY = XMLoadFloat3(&up);
	targetLocation = XMLoadFloat3(&target);

	newZ = targetLocation - currentLocation;
	newX = XMVector3Cross(globalY, newZ);
	newY = XMVector3Cross(newZ, newX);

	newX = XMVector3Normalize(newX);
	newY = XMVector3Normalize(newY);
	newZ = XMVector3Normalize(newZ);

	XMStoreFloat3(&xAxis, newX);
	XMStoreFloat3(&yAxis, newY);
	XMStoreFloat3(&zAxis, newZ);

	xAxis.y = 0.0f;
	yAxis.x = 0.0f;
	yAxis.z = 0.0f;
	zAxis.y = 0.0f;

	SetXAxis(xAxis);
	SetYAxis(yAxis);
	SetZAxis(zAxis);
}

void IObject::AddToRenderer()
{
	if (m_cpRenderMesh != nullptr)
	{
		GRAPHICS->AddRenderMesh(m_cpRenderMesh);
	}

	for (unsigned int i = 0; i < m_vpAttachments.size(); i++)
	{
		if (m_vpAttachments[i].mesh != nullptr)
			GRAPHICS->AddRenderMesh(m_vpAttachments[i].mesh);
	}

	for (unsigned int i = 0; i < m_vtPointLights.size(); i++)
	{
		if (&m_vtPointLights[i] != nullptr)
			GRAPHICS->AddLight(&m_vtPointLights[i]);
	}


}
