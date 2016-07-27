/***********************************************
* Filename:  		SpinningBlade.h
* Date:      		09/03/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Spinning blade that rotates around it's self and dealing damage to anything hit.
************************************************/
#ifndef CSPINNINGBLADE_H
#define CSPINNINGBLADE_H

#include "../../../Engine/Object Manager/IObject.h"

class CMesh;
class CSpinningBlade : public IObject
{
	int m_nTrapDamage;

	float m_fRotationSpeed;
	float m_fRotationAngle;

	float m_fRadius;

	bool m_bAudioPlaying = false;
	bool m_bMoving;

	XMFLOAT3 m_vStart;
	XMFLOAT3 m_vEnd;
	XMFLOAT3 m_vTarget;
	XMFLOAT3 m_fOffset;

	vector<bool> m_bDealDamageToPlayer;
	vector<bool> m_bDealDamageToSkeleton;
	CMesh* vSpinMesh;

	int m_nPasses = 0;
	CRenderMesh* m_cTrack = nullptr;

public:
	/*****************************************************************
	* Update()				Spinns the trap on it's Yaxis based on a rotation speed
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/02/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void Update();

	/*****************************************************************
	* CPressurePlate()		Initializes the class
	*
	* Ins:					XMFLOAT3
	*						float
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/03/2015
	* Mod. Initials:		NS
	*****************************************************************/
	CSpinningBlade(XMFLOAT3 _location, float _rotationSpeed, XMFLOAT3 _start = {0,0,0}, XMFLOAT3 _end = {0,0,0});

	/*****************************************************************
	* ~CSpinningBlade()		Cleans up memory / remove obj from objManager
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/03/2015
	* Mod. Initials:		NS
	*****************************************************************/
	~CSpinningBlade();


	/*****************************************************************
	* CollisionResponse()	Handles response for the spinning trap
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/03/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void CollisionResponse(IObject * _cCollidedObject);


	/*****************************************************************
	* FindBladeSize()		Goes through the mesh to find the length of the blades.
	*
	* Ins:
	*
	* Outs:					float
	*						float
	*
	* Returns:				void
	*
	* Mod. Date:		    09/03/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void FindBladeSize(float& _outMax, float& _outMin);

	/*********************************
	ACCESSORS
	************************************/
	int GetTrapDamage() const { return m_nTrapDamage; }
	float GetRotationAngle() const { return m_fRotationAngle; }
	float GetRotationSpeed() const { return m_fRotationSpeed; }
	float GetRadius() const { return m_fRadius; }
	XMFLOAT3& GetEndPoint() { return m_vEnd; }
	XMFLOAT3& GetStartPoint() { return m_vStart; }
	XMFLOAT3& GetTargetPoint() { return m_vTarget; }

	bool GetMoving() { return m_bMoving; }
	CRenderMesh* GetTrack() { return m_cTrack; }
	void CreateTrack();
};

#endif