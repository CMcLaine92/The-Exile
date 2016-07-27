#ifndef _BOUNDS_H_
#define _BOUNDS_H_

/***********************************************
* Filename:  		Bounds.h
* Date:      		08/12/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Defines the base class for all 
*                   bounding volumes
************************************************/

#include <DirectXMath.h>
using namespace DirectX;

#include <vector>

namespace Bounds
{
	enum eBoundsType { Base, AABB, Plane, Capsule, Sphere, Cylinder };
}

class CBounds
{
public:
	/// <summary>
	/// Returns type of Bounds the object is.
	/// </summary>
	/// <param> none</param>
	virtual Bounds::eBoundsType GetType();

	/// <summary>
	/// Updates the position of the bounds based on velocity
	/// </summary>
	/// <param name = "f3Velocity"> The bounds's velocity.</param>
	virtual void UpdatePosition(XMFLOAT3 f3Velocity);
	virtual void SetCenter(XMFLOAT3 pos){}


	virtual ~CBounds() = default;
};

#endif