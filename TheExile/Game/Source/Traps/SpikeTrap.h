/***********************************************
* Filename:  		SpikeTrap.h
* Date:      		09/03/2015
* Mod. Date: 		----------
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:   		Controles the behaviour of individual traps
************************************************/

#ifndef CSPIKETRAP_H
#define CSPIKETRAP_H

#include "../../../Engine/Object Manager/IObject.h"

#define SPIKE_PRESET_TIME 0.75f
#define SPIKE_PRESET_OFFSET_TIME 0.125f
#define SPIKE_PRESET_SPEED 250.0f

class CMesh;
class CObjectManager;
class CPressurePlate;

class CSpikeTrap : public IObject
{
	CPressurePlate* m_cpThePressurePlate = nullptr;

	//vector<CMesh> m_vMesh;

	int m_nTrapDamage;

	bool m_bPressurePlateActivated;

	bool m_bActive = false;
	bool m_bReset = false;
	bool m_bDealDamageToPlayer = false;
	bool m_bDamageDealtToPlayer = false;
	bool m_bDealDamageToSkeleton = false;
	bool m_bDamageDealtToSkeleton = false;
	bool m_bHasPressurePlate = false;
	bool m_bPresetOffset = false;

	float m_fActiveDelayTimer = 0.0f;
	float m_fResetDelayTimer = 0.0f;
	
	float m_fPresetTimer = 0.0f;
	float m_fPresetActiveTimer = 0.0f;

	float m_fStartTime = 0.0f;
	float m_fTimeInterval = 0.0f;
	float m_fCurrentTime = 0.0f;

public:
	/*****************************************************************
	* Update()				Updates the trap by translating it up and down based on a trigger or time
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
	* CSpikeTrap()			SpikeTrap constructor, sets up it's variables and creates a pressure plate.
	*
	* Ins:					XMFLOAT3
	*						bool
	*						float
	*						float
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/02/2015
	* Mod. Initials:	    NS
	*****************************************************************/
	CSpikeTrap(XMFLOAT3 _location, bool _pressurePlate, float _startTime, float _interval);

	/*****************************************************************
	* ~CSpikeTrap()			Destructor for the trap, cleans up allocated memory
	*
	* Ins:					CObjectManager*
	*						CAnimationManager*
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    08/20/2015
	* Mod. Initials:	    NS
	*****************************************************************/
	~CSpikeTrap();

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


	/*********************************
	ACCESSORS
	************************************/
	bool GetPressurePlateActivated() const { return m_bPressurePlateActivated; }
	bool GetReset() const { return m_bReset; }
	bool GetActive() const { return m_bActive; }
	int GetTrapDamage() const { return m_nTrapDamage; }

	/*********************************
	MUTATORS
	************************************/
	void MakeActive() { m_bActive = true; }
	void MakeInActive() { m_bActive = false; }

	void SetWorldMatrix(const DirectX::XMFLOAT4X4 matrix);

};

#endif