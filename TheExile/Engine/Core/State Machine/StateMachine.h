#pragma once
#include <queue>
#include <stack>
class CInputManager;
class CGameplayState;
class CMainMenuState;
class COptionState;
class CHowToPlayState;
class CCreditsState;
class CNickState;
class CLoadState;
#include "States/State.h"


class CStateMachine
{
	//enum eStateActions {eRemoveState, eAddState, NA};
	//queue<IState *> cNextState;
	std::stack<IState*> activeStates;

	CInputManager * Input;
	AudioSystemWwise m_AudioManager;

	CGameplayState* m_pGamePlayState;
	CMainMenuState* m_pMainMenuState;
	COptionState* m_pOptionsState;
	CHowToPlayState* m_pHowToPlayState;
	CCreditsState* m_pCreditState;
	CNickState* m_pNickState;
	CLoadState* m_pLoadState;
	bool m_bRunning;


	static CStateMachine * m_cpInstance;

public:

	
	//enum eStateType {eGameplayState, eMainMenuState, eOptionState, eHowToPlayState, eCreditsState, eDebug_NumiState, eDebug_HryshkoState, eDebug_NickState, eDebug_MattState, eDebug_SpencerState, eLoadState};

	CStateMachine();
	~CStateMachine();
	
	bool Update();

	void ClearStack();


	static CStateMachine * GetInstance();
	static void DeleteInstance();
	void PushState(eStateType newState);
	void PopState();
	const bool IsEmpty() const { return activeStates.empty(); }
	void Exit();
	bool Shutdown();
	bool Initialize();
};

