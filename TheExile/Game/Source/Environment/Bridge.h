/***********************************************
* Filename:  		CBridge.h
* Date:      		09/13/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Handles both transperant bridge and collidable bridge.
************************************************/

#ifndef BRIDGE_H
#define BRIDGE_H

#include "../../../Engine/Object Manager/IObject.h"
class CRenderMesh;
class CPlane;
class CMesh;
class CHUDMesh;

struct TPlank
{
	CRenderMesh* renderMesh = nullptr;
	CCollider* collider = nullptr;
	bool falling = false;
};

class CBridge : public IObject
{

	TPlank m_tPlanks[8];

	CRenderMesh* m_cpBuiltBase;

	CHUDMesh* m_cpTextFrame;
	CHUDMesh* m_cpControllerPopUp;



	int m_nFallingIndex;
	int m_nNumFallingPlanks;
	float m_fNextFallTimer;
	float m_fDelayTimer;
	bool m_bActive;
	bool m_bCollided;
	bool m_bCollapsing;
	bool m_bFalling;
	bool m_bIntact;

	bool m_bHasBeenBuilt;//able to repair if true

	CHUDMesh* m_cpBuildingBar;
	float m_fBuildTimer;
	float m_fBarLength;
	bool m_bBuildingBar;//true if in process of building


public:
	CBridge(XMFLOAT3 position = { 0.0f, 0.0f, 0.0f }, bool vertical = false);
	~CBridge();

	void Update();
	void Collapse();

	void SetWorldMatrix(const XMFLOAT4X4 world);
	bool IsIntact(){ return m_bIntact; }
	void Destroy();

	void Rebuild();		

	void InterruptBuild();
	bool BuildProcess();


	/*****************************************************************
	* DrawIndicator()		Draws the input red indicator for the bridge.
	*
	* Ins:					CPlane
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/16/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void DrawUnAvailableIndicator();

	/*****************************************************************
	* DrawIndicator()		Draws the input green indicator for the bridge.
	*
	* Ins:					CPlane
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/16/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void DrawAvailableIndicator();

	/*****************************************************************
	* FindOrientation()		Rotates the bridge on the Y according to pit size.
	*
	* Ins:					CPlane
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/15/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void FindOrientation(bool vertical);

	/*****************************************************************
	* Activate()			Activates the trap and makes it collidable.
	*
	* Ins:					None
	*
	* Outs:					None
	*
	* Returns:				void
	*
	* Mod. Date:		    09/15/2015
	* Mod. Initials:		NS
	*****************************************************************/
	void Activate();

	/*********************************
	ACCESSORS
	************************************/
	bool IsActive() const { return m_bActive; }
	bool GetRepairable() const { return m_bHasBeenBuilt; }

	/*********************************
	MUTATORS
	************************************/
	void SetActive(bool _active) { m_bActive = _active; }
	void SetCollided(bool _col) { m_bCollided = _col; }

	void AddToRenderer();
};

#endif 