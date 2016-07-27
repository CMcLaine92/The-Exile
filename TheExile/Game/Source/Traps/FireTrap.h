/***********************************************
* Filename:  		FireTrap.h
* Date:      		09/5/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Controls the trap that shoots fire when activated.
************************************************/

#ifndef CFIRETRAP_H
#define CFIRETRAP_H

#include "../../../Engine/Object Manager/IObject.h"

class CParticleManager;
class CObjectManager;
class CPressurePlate;

class CFireTrap : public IObject
{
	CObjectManager* m_cpObjManager = nullptr;

	CPressurePlate* m_cpPressurePlate = nullptr;
	
	int m_nTrapDamage;

	bool m_bActive = false;

	float m_fActiveTimer = 0.0f;
	float m_fDelayTimer = 0.0f;

	CParticleManager* m_cpParticleManager = nullptr;
	vector<unsigned int> m_vunParticle; 
	vector<unsigned int> m_vnSmokeParticle;
	XMFLOAT3 m_f3FireDirection;

public:

	/*****************************************************************
	* Update()				Updates the trap by shoting fire 5particles 
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/02/2015
	* Mod. Initials:
	*****************************************************************/
	void Update();

	/*****************************************************************
	* CFireTrap()			CFireTrap constructor, sets up it's variables and creates a pressure plate.
	*
	* Ins:					
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/02/2015
	* Mod. Initials:	    NS
	*****************************************************************/
	CFireTrap(CObjectManager* _objManager, XMFLOAT3 _location, XMFLOAT3 _facing);

	/*****************************************************************
	* ~CFireTrap()			Destructor for the trap, cleans up allocated memory
	*
	* Ins:					
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    08/20/2015
	* Mod. Initials:	    NS
	*****************************************************************/
	virtual ~CFireTrap();

	/*****************************************************************
	* CollisionResponse()	Handles collision response for the trap
	*
	* Ins:
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    08/20/2015
	* Mod. Initials:	    NS
	*****************************************************************/
	void CollisionResponse(IObject * _cCollidedObject);

	/*****************************************************************
	* InitializeParticles()	loads and sets up the particle for each trap
	*
	* Ins:					XMFLOAT3
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    08/20/2015
	* Mod. Initials:	    NS
	*****************************************************************/
	void InitializeParticles(XMFLOAT3 _heading);


	/*********************************
	ACCESSORS
	************************************/
	bool GetActive() const { return m_bActive; }
	int GetTrapDamage() const { return m_nTrapDamage; }

	/*********************************
	MUTATORS
	************************************/
	void MakeActive();
	void MakeInActive() { m_bActive = false; }

	void SetWorldMatrix(const DirectX::XMFLOAT4X4 matrix);

	IObject* GetPressurePlate() { return (IObject*)m_cpPressurePlate; };

	void AddToRenderer();

};

#endif