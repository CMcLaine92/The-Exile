#ifndef CLOAD_STATE
#define CLOAD_STATE

#include "State.h"

class IOjbect;
class CMesh;
class CHUDMesh;
class CCamera;
class CObjectManager;

class CLoadState : public IState
{

	IObject* m_cpCameraObject;
	CCamera* m_cpTheCamera;
	CMesh* m_cpCameraMesh;
	CHUDMesh* m_cpBackground;
	CHUDMesh* m_cpContinue;
	CHUDMesh* m_cpNewGame;
	CHUDMesh* m_cpContinueArrow;
	CHUDMesh* m_cpNewGameArrow;

	CObjectManager* m_cpObjManager;

	int m_nCursor;
	int prevCursor;
	bool m_bToGameplay;
public:
	virtual void Enter();
	virtual void Exit();
	CLoadState();
	~CLoadState();
	const bool ToGamePlay() const { return m_bToGameplay; }
	void Update();
	void Load();
	EInputResult Input();
	string GetFilePath();
	void BuildPlanes();
	eStateType GetStateType() { return eLOAD; }

private:

};


#endif