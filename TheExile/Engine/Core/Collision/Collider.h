#ifndef _COLLIDER_H_
#define _COLLIDER_H_

/***********************************************
* Filename:  		Collider.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines the collidable objects
*					that will be added to objects and
*                   used in collision detection and 
*                   collision responses
************************************************/


#include "Bounds.h"
#include <vector>
#include "../../Renderer/RenderMesh.h"
class CCollider
{
private:
	CBounds* m_cBounds;
	bool m_bCollided = false;
	bool m_bDynamic = false;
	bool m_bActive = false;
	//Should collide with Ray Casts
	bool m_bCatchRays = false;
	std::vector<IObject*> m_vpcCollidedObjects;

	XMFLOAT3 m_f3ResetVelocity = { 0, 0, 0 };
public:
	
	/**************************
		Accessors
	**************************/

	CBounds* GetBounds(void)
	{
		return m_cBounds;
	}
	bool GetCollided(void)
	{
		return m_bCollided;
	}
	bool GetDynamic(void)
	{
		return m_bDynamic;
	}
	bool GetActive(void)
	{
		return m_bActive;
	}
	bool GetCatchRays(void)
	{
		return m_bCatchRays;
	}

	vector<IObject*>& GetCollidedObjects(void)
	{
		return m_vpcCollidedObjects;
	}

	XMFLOAT3 GetResetVelocity(void)
	{
		return m_f3ResetVelocity;
	}

	/*********************************
		Mutators
	************************************/

	void SetBounds(CBounds* cBounds)
	{
		m_cBounds = cBounds;
	}
	void SetCollided(bool collided)
	{
		m_bCollided = collided;
	}
	void SetDynamic(bool bDynamic) 
	{
		m_bDynamic = bDynamic;
	}
	void SetActive(bool bActive)
	{
		m_bActive = bActive;
	}
	void SetGetRays(bool bCatchRays)
	{
		m_bCatchRays = bCatchRays;
	}
	void SetResetVelocity(XMFLOAT3 f3Velocity);

	/// <summary>
	/// Deletes the bounds variable if not nullptr.
	/// </summary>
	/// <param> none</param>
	~CCollider();

	/// <summary>
	/// Constructor to initialize all data.
	/// </summary>
	/// <param name = "bDynamic"> If the collider will be moving.</param>
	/// <param name = "cBounds"> A pointer to the bounds object.</param>
	CCollider(bool bDynamic, CBounds* cBounds, bool m_bActive = true, bool bCatchRays = true);

	/// <summary>
	/// Constructor to initialize all data.
	/// </summary>
	/// <param name = "bDynamic"> If the collider will be moving.</param>
	/// <param name = "eType"> The type of bounds it is.</param>
	/// <param name = "vVertices"> The vertices to be encapsulated by the bounds.</param>
	CCollider(bool bDynamic, Bounds::eBoundsType eType, std::vector<TVertex> vVertices,bool m_bActive = true, bool bCatchRays = true);

	/// <summary>
	/// Updates the position of the collider based on velocity
	/// </summary>
	/// <param name = "f3Velocity"> The collider's velocity.</param>
	void UpdatePosition(XMFLOAT3 f3Velocity);

	/// <summary>
	/// Returns type of Bounds the object is.
	/// </summary>
	/// <param> none</param>
	Bounds::eBoundsType GetType();
};




#endif