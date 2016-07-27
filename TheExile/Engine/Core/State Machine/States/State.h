
#pragma once
enum eStateType { eGAMEPLAY, eMAINMENU, eOPTIONS, eCONTROLS, eCREDITS, eLOAD, eNONE };
enum EInputResult { eContinue, eRemove, eAdd, eAddGamePlay };

class IState
{
private:

	unsigned int m_nStateType;

public:



	virtual EInputResult Input();
	virtual void Update();
	virtual void ReInitialize();

	virtual void Enter();
	virtual void Exit();

	IState();
	virtual ~IState() = default;

	virtual eStateType GetStateType() { return eNONE; }
};

