#include "StateMachine.h"
#include <cassert>
#include "../Debug Console/DebugConsole.h"

//State includes
#include "../../Core/State Machine/States/GameplayState.h"
#include "../../Core/State Machine/States/HowToPlayState.h"
#include "../../Core/State Machine/States/MainMenuState.h"
#include "../../Core/State Machine/States/NickState.h"
#include "../../Core/State Machine/States/OptionState.h"
#include "../../Core/State Machine/States/CreditsState.h"
#include "../../Core/State Machine/States/LoadState.h"
#include "../../Core/Input/InputManager.h"

#include "../../Asset Manager/AssetManager.h"

CStateMachine * CStateMachine::m_cpInstance = nullptr;

/*****************************************************************
* CstateMachine():		Initialize the state machine
*
* Ins:					N/A
*
* Outs:					N/A
*
* Returns:				N/A
*
* Mod. Date:			8/11/2015
* Mod. Initials:		SS
*****************************************************************/
CStateMachine::CStateMachine()
{
	/*m_pGamePlayState = nullptr;
	m_pMainMenuState = nullptr;
	m_pOptionsState = nullptr;
	m_pHowToPlayState = nullptr;
	m_pCreditState = nullptr;
	m_pNickState = nullptr;
	m_pLoadState = nullptr;
	Input = nullptr;
	m_bRunning = false;*/
}

CStateMachine* CStateMachine::GetInstance()
{
	if (m_cpInstance == nullptr)
	{
		m_cpInstance = new CStateMachine;
		m_cpInstance->Initialize();
	}

	return m_cpInstance;
}

void CStateMachine::Exit()
{
	
}

void CStateMachine::DeleteInstance()
{
	if (m_cpInstance == nullptr)
	{
		return;
	}

	if (m_cpInstance->Shutdown() == false)
	{
		//send error message
	}

	SAFE_DELETE(m_cpInstance);
	
}

bool CStateMachine::Shutdown()
{
	Input->DeleteInstance();
	CAssetManager::GetInstance()->DeleteInstance();

	m_AudioManager.UnloadSoundBank(L"Init.bnk");
	m_AudioManager.UnloadSoundBank(L"SoundBank.bnk");
	m_AudioManager.Shutdown();

	


	SAFE_DELETE(m_pGamePlayState);
	SAFE_DELETE(m_pMainMenuState);
	SAFE_DELETE(m_pOptionsState);
	SAFE_DELETE(m_pHowToPlayState);
	SAFE_DELETE(m_pCreditState);
	SAFE_DELETE(m_pNickState);
	SAFE_DELETE(m_pLoadState);


	
	return true;
}

bool CStateMachine::Initialize()
{
	//	SetupDebugConsole();
	Input = CInputManager::GetInstance();

	// - Initializing the Audio Wrapper
	m_AudioManager.Initialize();
	m_AudioManager.SetBasePath(L"../Game/Assets/Audio/");
	m_AudioManager.LoadSoundBank(L"Init.bnk");
	m_AudioManager.LoadSoundBank(L"SoundBank.bnk");

	m_pGamePlayState = new CGameplayState;
	m_pMainMenuState = new CMainMenuState;
	m_pOptionsState = new COptionState;
	m_pHowToPlayState = new CHowToPlayState;
	m_pCreditState = new CCreditsState;
	m_pNickState = new CNickState;
	m_pLoadState = new CLoadState;
	m_bRunning = true;
	return true;
}


/*****************************************************************
* ~CstateMachine():		Deallocate all dynamic memory
*
* Ins:					N/A
*
* Outs:					N/A
*
* Returns:				N/A
*
* Mod. Date:			8/11/2015
* Mod. Initials:		SS
*****************************************************************/
CStateMachine::~CStateMachine()
{

}

/*****************************************************************
* AddState():		Queues up the passed in state type to be added
*					to the stack after the next update
*
* Ins:				eStateType _eType
*
* Outs:				N/A
*
* Returns:			N/A
*
* Mod. Date:		8/11/2015
* Mod. Initials:	SS
*****************************************************************/


/*****************************************************************
* RemoveState():	Removes the state after the next update
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

void CStateMachine::PushState(eStateType newState)
{
	switch (newState)
	{
	case eGAMEPLAY:
		m_pGamePlayState->Enter();
		activeStates.push(m_pGamePlayState);
		break;
	case eMAINMENU:
		m_pMainMenuState->Enter();
		activeStates.push(m_pMainMenuState);
		break;
	case eOPTIONS:
		m_pOptionsState->Enter();
		activeStates.push(m_pOptionsState);
		break;
	case eCONTROLS:
		m_pHowToPlayState->Enter();
		activeStates.push(m_pHowToPlayState);
		break;
	case eCREDITS:
		m_pCreditState->Enter();
		activeStates.push(m_pCreditState);
		break;
	case eNONE:
		m_pNickState->Enter();
		activeStates.push(m_pNickState);
		break;
	case eLOAD:
		m_pLoadState->Enter();
		activeStates.push(m_pLoadState);
		break;
	default:
		break;
	}
}
void CStateMachine::PopState()
{
	if (!activeStates.empty())
	{
		eStateType stateType = activeStates.top()->GetStateType();
		
		switch (stateType)
		{
		case eGAMEPLAY:
		{
			bool gameOver = m_pGamePlayState->IsGameOver();
			bool playerLose = m_pGamePlayState->DidPlayerWin();
			activeStates.top()->Exit();
			activeStates.pop();
			if (gameOver == true)
			{
				if (playerLose == true)
				{
					m_pMainMenuState->Enter();
					activeStates.push(m_pMainMenuState);
				}
				else
				{
					m_pCreditState->Enter();
					activeStates.push(m_pCreditState);
				}
			}
			else
			{
				m_pMainMenuState->Enter();
				activeStates.push(m_pMainMenuState);
			}
		}
			break;
		case eOPTIONS:
		{
			activeStates.top()->Exit();
			activeStates.pop();

			if (activeStates.empty() == false && activeStates.top()->GetStateType() == eMAINMENU)
			{
				m_pMainMenuState->ReInitialize();
			}
			if (activeStates.empty() == false && activeStates.top()->GetStateType() == eGAMEPLAY)
			{
				m_pGamePlayState->ReInitialize();
			}
		}
		break;
		case eCREDITS:
			ClearStack();
			m_pMainMenuState->Enter();
			activeStates.push(m_pMainMenuState);
			break;
		case eCONTROLS:
			activeStates.top()->Exit();
			activeStates.pop();
			m_pMainMenuState->ReInitialize();

			break;
		case eMAINMENU:

			if (m_pMainMenuState->ToGameplay())
			{
				activeStates.top()->Exit();
				activeStates.pop();
				m_pLoadState->Enter();
				activeStates.push(m_pLoadState);

			}
			else
			{
				activeStates.top()->Exit();
				activeStates.pop();
			}
			break;
		case eLOAD:
			if (m_pLoadState->ToGamePlay())
			{
				ClearStack();

				activeStates.push(m_pGamePlayState);
			}
			else
			{
				GRAPHICS->ClearScene();
				activeStates.top()->Exit();
				activeStates.pop();
				m_pMainMenuState->Enter();
				activeStates.push(m_pMainMenuState);
			}
			break;
		default:
			break;
		}
	}

	if (activeStates.empty())
		m_bRunning = false;
	
}
/*****************************************************************
* Update():			Handles updating the current state and
*					switching states at the correct point
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
bool CStateMachine::Update()
{
	m_AudioManager.Update();
	Input->Update();
	IState* m_pCurrState = activeStates.top();
	
	if (m_pCurrState)
	{
		EInputResult result = m_pCurrState->Input();
		switch (result)
		{
		case eContinue:
			m_pCurrState->Update();
			return true;
			break;
		case eRemove:
			PopState();
			break;
		default:
			break;
		}
	}

	return false;
}
void CStateMachine::ClearStack()
{
	while (!activeStates.empty())
	{
		activeStates.top()->Exit();
		activeStates.pop();
	}
}


