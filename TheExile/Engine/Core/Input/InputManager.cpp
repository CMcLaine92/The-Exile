/***********************************************
* Filename:  		InputManger.cpp
* Date:      		8/7/2015
* Mod. Date: 		8/7/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		Detects and manages keyboard,
*					mouse, and controller input
************************************************/

#include "InputManager.h"
#include "../../Renderer/Renderer.h"

CInputManager* CInputManager::m_cpInstance = nullptr;



/*	PRIVATE FUNCTIONS	*/

/*****************************************************************
* Initialize()			Initializes the InputManager keyboard, mouse,
*						and controller
*
* Ins:					None
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::Initialize()
{
	//get active window
	m_InputWindow = GetActiveWindow();
	if (m_InputWindow == nullptr)
	{
		return false;
	}

	//initialize variables
	memset(m_chKeyboard, 0, sizeof(m_chKeyboard));

	m_f2MouseDirection = XMFLOAT2(0.0f, 0.0f);


	//get mouse position
	GetCursorPos(&m_ptMousePosition);
	ScreenToClient(m_InputWindow, &m_ptMousePosition);

	//clip mouse position
	RECT tempRect = {};
	GetClipCursor(&tempRect);
	m_ptMousePosition.x -= tempRect.left;
	m_ptMousePosition.y -= tempRect.top;


	//controller
	m_bIsControllerConnected = CheckForControllers();
	m_fControllerCheckTimer = 0.0f;
	m_bControllerBeingUsed = m_bIsControllerConnected;
	m_bControllerDisconnected = false;

	m_bAxisLocked = false;
	m_fAxisHoldDelayTimer = CONTROLLER_AXIS_HOLD_DELAY;
	m_fAxisPressedDelayTimer = CONTROLLER_AXIS_PRESSED_DELAY;

	// Register Full Screen Key //
	RegisterKey("Full Screen", eKey::F11);
	RegisterKey("Alt", eKey::Alt);
	RegisterKey("Alt_Enter", eKey::Enter);

	return true;
}

/*****************************************************************
* Shutdown()			Shuts down the InputManager
*
* Ins:					None
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::Shutdown()
{
	memset(m_chKeyboard, 0, sizeof(m_chKeyboard));

	return true;
}

/*****************************************************************
* CheckForControllers()	Checks for conected controllers and sets the
first one as the controller for use
*
* Ins:					None
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::CheckForControllers()
{
	//check if a controller is connected
	int NumConnected = 0;
	for (unsigned int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		DWORD result = XInputGetState(i, &state);
		if (result == ERROR_SUCCESS)
		{
			if (NumConnected == 0)//only save first controller
			{
				m_tController.m_currentState = state;
				m_tController.m_prevPacketNum = m_tController.m_currentState.dwPacketNumber;
				m_tController.m_unUserIndex = i;
				m_tController.m_bVibrating = false;
				m_tController.m_fVibrateTime = 0.0f;
				m_tController.m_unRightMotorSpeed = 0;
				m_tController.m_unLeftMotorSpeed = 0;
			}

			NumConnected++;
		}
	}

	if (NumConnected > 0)
	{
		return true;
	}

	return false;
}

/*****************************************************************
* GetAxisValue()		Returns the axis value of the specified type
*
* Ins:					axisType
*
* Outs:					None
*
* Returns:				short
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
short CInputManager::GetAxisValue(eAxis axisType) const
{
	switch (axisType)
	{
	case LeftX:
	{
		return m_tController.m_currentState.Gamepad.sThumbLX;
	}
	break;
	case LeftY:
	{
		return m_tController.m_currentState.Gamepad.sThumbLY;
	}
	break;
	case RightX:
	{
		return m_tController.m_currentState.Gamepad.sThumbRX;
	}
	break;
	case RightY:
	{
		return m_tController.m_currentState.Gamepad.sThumbRY;
	}
	break;
	}

	return 0;
}



/*	PUBLIC FUNCTIONS	*/

/*****************************************************************
* GetInstance()			Returns the current instance of the InputManager
*
* Ins:					None
*
* Outs:					None
*
* Returns:				CInputManger*
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
CInputManager* CInputManager::GetInstance()
{
	if (m_cpInstance == nullptr)
	{
		m_cpInstance = new CInputManager;
		if (m_cpInstance->Initialize() == false)
		{
			//send error message
			return m_cpInstance;
		}
	}

	return m_cpInstance;
}

/*****************************************************************
* DeleteInstance()		Deletes the current instance of the InputManager
*
* Ins:					None
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
void CInputManager::DeleteInstance()
{
	if (m_cpInstance == nullptr)
	{
		return;
	}

	if (m_cpInstance->Shutdown() == false)
	{
		//send error message
	}

	delete m_cpInstance;
	m_cpInstance = nullptr;
}

/*****************************************************************
* Update()				Updates the InputManager's keyboard, mouse,
*						and controller status
*
* Ins:					None
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::Update()
{
	// Full Screen Check //
	IsKeyPressed("Alt_Enter");
	if (IsKeyPressed("Full Screen"))
		GRAPHICS->ToggleFullScreen();


#pragma region Keyboard Update

	//keyboard
	unsigned char tempKeyboard[NUMBER_OF_KEYS];

	if (GetForegroundWindow() == m_InputWindow)
	{
		BOOL errorCheck = GetKeyboardState(tempKeyboard);
		if (errorCheck == false)
		{
			return false;
		}

		//FlushKeys();
		for (unsigned int i = 0; i < NUMBER_OF_KEYS; i++)
		{
			m_chKeyboard[i] = ((m_chKeyboard[i] & eBit_Key_States::Pressed_State) >> 1) | (tempKeyboard[i]);
		}

	}
	else
	{
		FlushKeys();
	}

#pragma endregion

#pragma region Mouse Update

	//if a controller is being used, cancel all mouse movement
	if (m_bControllerBeingUsed == false)
	{
		//mouse update
		if (GetForegroundWindow() == m_InputWindow)
		{
			POINT tempMouse;
			GetCursorPos(&tempMouse);

			m_f2MouseDirection = XMFLOAT2{ float(tempMouse.x - m_ptMousePosition.x), float(tempMouse.y - m_ptMousePosition.y) };

			// Cursor Wrap 
			RECT clientRect;
			GetWindowRect(m_InputWindow, &clientRect);

			//if (tempMouse.x < clientRect.left + 2) // Left
			//{
			//	SetCursorPos(clientRect.right - 2, tempMouse.y);
			//	GetCursorPos(&tempMouse);
			//}
			//if (tempMouse.y < clientRect.top +2) // Top
			//{
			//	SetCursorPos(tempMouse.x, clientRect.bottom-2);
			//	GetCursorPos(&tempMouse);
			//}
			//if (tempMouse.x > clientRect.right - 2) // Right
			//{
			//	SetCursorPos(clientRect.left + 2, tempMouse.y);
			//	GetCursorPos(&tempMouse);
			//}
			//if (tempMouse.y > clientRect.bottom - 2) // Bottom
			//{
			//	SetCursorPos(tempMouse.x, clientRect.top + 2);
			//	GetCursorPos(&tempMouse);
			//}

			SetCursorPos((clientRect.left + clientRect.right) / 2, (clientRect.top + clientRect.bottom) / 2);
			GetCursorPos(&tempMouse);
			m_ptMousePosition = tempMouse;
		}
	}

#pragma endregion

#pragma region Controller Update

	//controller
	if (m_bIsControllerConnected == true)
	{
		m_tController.m_previousState = m_tController.m_currentState;
		DWORD connection = XInputGetState(m_tController.m_unUserIndex, &m_tController.m_currentState);
		if (connection != ERROR_SUCCESS)
		{
			m_bControllerBeingUsed = false;
			m_bIsControllerConnected = false;
			m_bControllerDisconnected = true;
			return true;
		}

		//the controller state has changed since last frame
		if (m_tController.m_currentState.dwPacketNumber != m_tController.m_prevPacketNum)
		{
#pragma region Axis 

			for (unordered_map<string, TInputAxis>::iterator iter = m_RegisteredAxis.begin(); iter != m_RegisteredAxis.end(); iter++)
			{
				short tempAxisValue = GetAxisValue((*iter).second.m_eType);
				bool isAxisDown = false;

				if ((*iter).second.m_nDir > 0)//its a pos axis
				{
					if (tempAxisValue > AXIS_DEADZONE)
					{
						(*iter).second.m_fAxisValue = (float)tempAxisValue;// / AXIS_DEADZONE_LERP_MAX_VALUE;
						isAxisDown = true;
					}
					else
						(*iter).second.m_fAxisValue = 0.0f;
				}
				else//its neg axis
				{
					if (tempAxisValue < -AXIS_DEADZONE)
					{
						(*iter).second.m_fAxisValue = (float)tempAxisValue;// / AXIS_DEADZONE_LERP_MAX_VALUE;
						isAxisDown = true;
					}
					else
						(*iter).second.m_fAxisValue = 0.0f;
				}

				switch ((*iter).second.m_eCurrentState)
				{
				case eButtonStates::NONE:
				{
					if (isAxisDown == true)
					{
						(*iter).second.m_ePreviousState = eButtonStates::NONE;
						(*iter).second.m_eCurrentState = eButtonStates::PRESSED;
					}
					else
					{
						(*iter).second.m_ePreviousState = eButtonStates::NONE;
						(*iter).second.m_eCurrentState = eButtonStates::NONE;
					}
				}
				break;
				case eButtonStates::PRESSED:
				{
					if (isAxisDown == true)
					{
						(*iter).second.m_ePreviousState = eButtonStates::PRESSED;
						(*iter).second.m_eCurrentState = eButtonStates::DOWN;
					}
					else
					{
						(*iter).second.m_ePreviousState = eButtonStates::PRESSED;
						(*iter).second.m_eCurrentState = eButtonStates::NONE;
					}
				}
				break;
				case eButtonStates::DOWN:
				{
					if (isAxisDown == true)
					{
						(*iter).second.m_ePreviousState = eButtonStates::DOWN;
						(*iter).second.m_eCurrentState = eButtonStates::DOWN;
					}
					else
					{
						(*iter).second.m_ePreviousState = eButtonStates::DOWN;
						(*iter).second.m_eCurrentState = eButtonStates::NONE;
					}
				}
				break;
				default:
					break;
				}
				
			}

#pragma endregion

#pragma region Triggers

			for (auto i = m_RegisteredTriggers.begin(); i != m_RegisteredTriggers.end(); i++)
			{
				if ((*i).second.m_eType == eTriggers::RightTrigger)
					(*i).second.m_fThreshHold = m_tController.m_currentState.Gamepad.bRightTrigger;
				else if ((*i).second.m_eType == eTriggers::LeftTrigger)
					(*i).second.m_fThreshHold = m_tController.m_currentState.Gamepad.bLeftTrigger;

				bool bTriggerDown = false;
				if ((*i).second.m_fThreshHold > TRIGGER_THRESHOLD)
					bTriggerDown = true;

				switch ((*i).second.m_eCurrentState)
				{
				case NONE:
				{
					if (bTriggerDown == true)
					{
						(*i).second.m_ePreviousState = eButtonStates::NONE;
						(*i).second.m_eCurrentState = eButtonStates::PRESSED;
					}
					else
					{
						(*i).second.m_ePreviousState = eButtonStates::NONE;
						(*i).second.m_eCurrentState = eButtonStates::NONE;
					}
				}
				break;
				case PRESSED:
				{
					if (bTriggerDown == true)
					{
						(*i).second.m_ePreviousState = eButtonStates::PRESSED;
						(*i).second.m_eCurrentState = eButtonStates::DOWN;
					}
					else
					{
						(*i).second.m_ePreviousState = eButtonStates::PRESSED;
						(*i).second.m_eCurrentState = eButtonStates::NONE;
					}
				}
				break;
				case DOWN:
				{
					if (bTriggerDown == false)
					{
						(*i).second.m_ePreviousState = eButtonStates::DOWN;
						(*i).second.m_eCurrentState = eButtonStates::NONE;
					}
					else
					{
						(*i).second.m_ePreviousState = eButtonStates::DOWN;
						(*i).second.m_eCurrentState = eButtonStates::DOWN;
					}
				}
				break;
				default:
					break;
				}
			}

#pragma endregion

		}

		m_tController.m_prevPacketNum = m_tController.m_currentState.dwPacketNumber;
	}
	else
	{
		m_fControllerCheckTimer += DELTA_TIME();
		if (m_fControllerCheckTimer > CONTROLLER_CHECK_TIME)
		{
			m_bIsControllerConnected = CheckForControllers();
			if (m_bIsControllerConnected == true)
				m_bControllerDisconnected = false;

			m_fControllerCheckTimer = 0.0f;
		}
	}

	if (m_bAxisLocked == true)
	{
		m_fAxisPressedDelayTimer += DELTA_TIME();
		if (m_fAxisPressedDelayTimer >= CONTROLLER_AXIS_PRESSED_DELAY)
		{
			m_fAxisPressedDelayTimer = 0.0f;
			m_bAxisLocked = false;
		}
	}

#pragma region Vibration

	//vibrate timer
	if (m_tController.m_bVibrating == true)
	{
		//vibrate the controller
		ZeroMemory(&m_tController.m_vibration, sizeof(XINPUT_VIBRATION));
		m_tController.m_vibration.wLeftMotorSpeed = (WORD)m_tController.m_unLeftMotorSpeed;
		m_tController.m_vibration.wRightMotorSpeed = (WORD)m_tController.m_unRightMotorSpeed;
		XInputSetState(m_tController.m_unUserIndex, &m_tController.m_vibration);

		m_tController.m_fVibrateTime -= DELTA_TIME();
		if (m_tController.m_fVibrateTime <= 0.0f)
		{
			ZeroMemory(&m_tController.m_vibration, sizeof(XINPUT_VIBRATION));
			m_tController.m_vibration.wLeftMotorSpeed = 0;
			m_tController.m_vibration.wRightMotorSpeed = 0;
			XInputSetState(m_tController.m_unUserIndex, &m_tController.m_vibration);

			m_tController.m_bVibrating = false;
			m_tController.m_fVibrateTime = 0.0f;
		}
	}

#pragma endregion

#pragma endregion

	return true;
}

/*****************************************************************
* RegisterKey()			Registers a name to a key or multiple keys
*
* Ins:					Name
*						key1
*						key2
*						key3
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
void CInputManager::RegisterKey(string name, eKey key1, eKey key2, eKey key3)
{
	vector<eKey> newKeys;
	unordered_map<string, vector<eKey>>::iterator iter = m_RegisteredKeys.find(name);
	if (iter != m_RegisteredKeys.end())
	{
		newKeys = (*iter).second;
		m_RegisteredKeys.erase(iter);
	}

	newKeys.push_back(key1);

	if (key2 != eKey::None)
		newKeys.push_back(key2);

	if (key3 != eKey::None)
		newKeys.push_back(key3);

	m_RegisteredKeys.insert(pair<string, vector<eKey>>(name, newKeys));
}

/*****************************************************************
* IsKeyPressed()		Checks if the passed in name's registered
*						key is pressed
*
* Ins:					Name
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::IsKeyPressed(string name)
{
	if (m_RegisteredKeys.find(name) != m_RegisteredKeys.end())
	{
		vector<eKey>& vKeys = m_RegisteredKeys[name];
		for (unsigned int i = 0; i < (unsigned int)vKeys.size(); i++)
		{
			if (vKeys[i] == Enter && (m_chKeyboard[vKeys[i]] & Bit_Offset) == Pressed_State && IsKeyDown("Alt"))
			{
				if (name == "Alt_Enter")
					GRAPHICS->ToggleFullScreen();

				return false;
			}
			else if ((m_chKeyboard[vKeys[i]] & Bit_Offset) == Pressed_State)
				return true;
		}
	}

	return false;
}

/*****************************************************************
* IsKeyDown()			Checks if the passed in name's registered
*						key is down
*
* Ins:					Name
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::IsKeyDown(string name)
{
	if (m_RegisteredKeys.find(name) != m_RegisteredKeys.end())
	{
		vector<eKey>& v_Keys = m_RegisteredKeys[name];
		int size = (int)v_Keys.size();
		for (int currKey = 0; currKey < size; currKey++)
		{
			if ((m_chKeyboard[v_Keys[currKey]] & Pressed_State) == Pressed_State)
				return true;
		}
	}
		
	return false;
}

/*****************************************************************
* RegisterButton()		Registers a name to a button or multiple buttons
*
* Ins:					Name
*						button1
*						button2
*						button3
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
void CInputManager::RegisterButton(string name, eButton button1, eButton button2, eButton button3)
{
	vector<eButton> newButtons;
	unordered_map<string, vector<eButton>>::iterator iter = m_RegisteredButtons.find(name);
	if (iter != m_RegisteredButtons.end())
		m_RegisteredButtons.erase(iter);
	
	newButtons.push_back(button1);

	if (button2 != eButton::NoButton)
		newButtons.push_back(button2);

	if (button3 != eButton::NoButton)
		newButtons.push_back(button3);

	m_RegisteredButtons.insert(pair<string, vector<eButton>>(name, newButtons));
}

/*****************************************************************
* RegisterAxis()		Registers 2 names for a axis: one for positive
values and one for negative values
*
* Ins:					negName
*						posName
*						button
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
void CInputManager::RegisterAxis(string negName, string posName, eAxis axis)
{
	bool insertNeg = true;
	bool insertPos = true;
	unordered_map<string, TInputAxis>::iterator posIter = m_RegisteredAxis.find(posName);
	unordered_map<string, TInputAxis>::iterator negIter = m_RegisteredAxis.find(negName);

	if (negIter != m_RegisteredAxis.end())
		insertNeg = false;

	if (posIter != m_RegisteredAxis.end())
		insertPos = false;

	if (insertPos == false && insertPos == false)
		return;

	TInputAxis negAxis;
	negAxis.m_nDir = -1;
	negAxis.m_eType = axis;
	negAxis.m_eCurrentState = eButtonStates::NONE;
	negAxis.m_ePreviousState = eButtonStates::NONE;
	negAxis.m_fAxisValue = 0.0f;

	TInputAxis posAxis;
	posAxis.m_nDir = 1;
	posAxis.m_eType = axis;
	posAxis.m_eCurrentState = eButtonStates::NONE;
	posAxis.m_ePreviousState = eButtonStates::NONE;
	posAxis.m_fAxisValue = 0.0f;

	m_RegisteredAxis.insert(pair<string, TInputAxis>(negName, negAxis));
	m_RegisteredAxis.insert(pair<string, TInputAxis>(posName, posAxis));

}

/*****************************************************************
* IsButtonPressed()		Checks if the passed in name's registered
*						button is pressed
*
* Ins:					name
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::IsButtonPressed(string name)
{
	if (m_bControllerBeingUsed == false)
		return false;

	unordered_map<string, vector<eButton>>::iterator iter = m_RegisteredButtons.find(name);
	if (iter != m_RegisteredButtons.end())
	{
		vector<eButton>& v_Buttons = iter->second;
		int size = (int)v_Buttons.size();
		for (int _button = 0; _button < size; _button++)
		{
			if ((m_tController.m_currentState.Gamepad.wButtons & v_Buttons[_button]) != 0 && (m_tController.m_previousState.Gamepad.wButtons & v_Buttons[_button]) == 0)
				return true;
		}
	}

	return false;
}

/*****************************************************************
* IsButtonDown()		Checks if the passed in name's registered
*						button is down
*
* Ins:					name
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::IsButtonDown(string name)
{
	if (m_bControllerBeingUsed == false)
		return false;

	unordered_map<string, vector<eButton>>::iterator iter = m_RegisteredButtons.find(name);

	if (iter != m_RegisteredButtons.end())
	{
		vector<eButton>& v_Buttons = iter->second;
		int size = v_Buttons.size();
		for (int _button = 0; _button < size; _button++)
		{
			if (((m_tController.m_currentState.Gamepad.wButtons & v_Buttons[_button]) != 0) && ((m_tController.m_previousState.Gamepad.wButtons & v_Buttons[_button]) != 0))
				return true;
		}
	}

	return false;
}

/*****************************************************************
* IsAxisDown()			Checks if the passed in name's registered
*						axis is down
*
* Ins:					name
*
* Outs:					None
*
* Returns:				bool
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
bool CInputManager::IsAxisDown(string name)
{
	if (m_bControllerBeingUsed == false)
		return false;

	//axis
	unordered_map<string, TInputAxis>::iterator iter = m_RegisteredAxis.find(name);
	if (iter != m_RegisteredAxis.end())
	{
		if ((*iter).second.m_eCurrentState == eButtonStates::DOWN && (*iter).second.m_ePreviousState == eButtonStates::DOWN)
			return true;
	}
	
	return false;
}

bool CInputManager::IsAxisPressed(string name)
{
	if (m_bControllerBeingUsed == false)
		return false;
	
	if (m_bAxisLocked == false)
	{
		unordered_map<string, TInputAxis>::iterator iter = m_RegisteredAxis.find(name);
		if (iter != m_RegisteredAxis.end())
		{
			if ((*iter).second.m_eCurrentState == eButtonStates::PRESSED && (*iter).second.m_ePreviousState == eButtonStates::NONE)
			{
				m_bAxisLocked = true;
				return true;
			}
		}
	}

	//delay joystick response
	if (IsAxisDown(name) == true)
	{
		m_fAxisHoldDelayTimer += DELTA_TIME();
		if (m_fAxisHoldDelayTimer >= CONTROLLER_AXIS_HOLD_DELAY)
		{
			m_fAxisHoldDelayTimer = 0.0f;
			return true;
		}
	}

	return false;
}

/*****************************************************************
* VibrateController()	Vibrates the controller
*
* Ins:					name
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
void CInputManager::VibrateController(unsigned int leftMotor, unsigned int rightMotor, float vibrateTime)
{
	if (m_bIsControllerConnected == false)
		return;

	m_tController.m_fVibrateTime = vibrateTime;
	m_tController.m_unRightMotorSpeed = rightMotor;
	m_tController.m_unLeftMotorSpeed = leftMotor;
	m_tController.m_bVibrating = true;

	//ZeroMemory(&m_tController.m_vibration, sizeof(XINPUT_VIBRATION));
	//m_tController.m_vibration.wLeftMotorSpeed = (WORD)leftMotor;
	//m_tController.m_vibration.wRightMotorSpeed = (WORD)rightMotor;
	//XInputSetState(m_tController.m_unUserIndex, &m_tController.m_vibration);
}

void CInputManager::FlushKeys()
{
	memset(m_chKeyboard, 0, sizeof(m_chKeyboard));
	SetKeyboardState(m_chKeyboard);
}

void CInputManager::RegisterTrigger(string name, eTriggers trigger)
{
	TTrigger tempTrigger;
	tempTrigger.m_eType = trigger;
	tempTrigger.m_fThreshHold = 0;
	tempTrigger.m_eCurrentState = NONE;
	tempTrigger.m_ePreviousState = NONE;

	m_RegisteredTriggers.insert(pair<string, TTrigger>(name, tempTrigger));
}

bool CInputManager::IsTriggerDown(string name)
{
	if (m_bControllerBeingUsed == false)
		return false;

	//axis
	unordered_map<string, TTrigger>::iterator iter = m_RegisteredTriggers.find(name);
	if (iter != m_RegisteredTriggers.end())
	{
		if ((*iter).second.m_eCurrentState == eButtonStates::DOWN && (*iter).second.m_ePreviousState == eButtonStates::DOWN)
			return true;
	}

	return false;
}

bool CInputManager::IsTriggerPressed(string name)
{
	if (m_bControllerBeingUsed == false)
		return false;

	unordered_map<string, TTrigger>::iterator iter = m_RegisteredTriggers.find(name);
	//axis
	if (iter != m_RegisteredTriggers.end())
	{
		if ((*iter).second.m_eCurrentState == eButtonStates::PRESSED && (*iter).second.m_ePreviousState == eButtonStates::NONE)
			return true;
	}

	return false;
}

bool CInputManager::DidControllerDisconnect()
{
	return m_bControllerDisconnected;
}

bool CInputManager::IsControllerConnected()
{
	return m_bIsControllerConnected;
}

float CInputManager::GetRegisteredAxisValue(string name)
{
	if (m_bIsControllerConnected == false)
		return 0.0f;

	unordered_map<string, TInputAxis>::iterator iter = m_RegisteredAxis.find(name);
	if (iter != m_RegisteredAxis.end())
		return (*iter).second.m_fAxisValue;

	return 0.0f;
}


/*	ACCESSORS	*/

/*****************************************************************
* GetMouseDirection()	Returns the mouse movement direction
*
* Ins:					None
*
* Outs:					None
*
* Returns:				XMFLOAT2
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
XMFLOAT2 CInputManager::GetMouseDirection() const
{
	return m_f2MouseDirection;
}

bool CInputManager::GetControllerBeingUsed() const
{
	return m_bControllerBeingUsed;
}



/*	MUTATORS	*/

/*****************************************************************
* SetDPad():			Registers the 4 strings as the 4 DPad directions
*
* Ins:					left
*						right
*						up
*						down
*
* Outs:					None
*
* Returns:				void
*
* Mod. Date:		    08/07/2015
* Mod. Initials:	    NH
*****************************************************************/
void CInputManager::SetDPad(string left, string right, string up, string down)
{
	m_RegisteredDPad[left] = eButton::DpadLeft;
	m_RegisteredDPad[right] = eButton::DpadRight;
	m_RegisteredDPad[up] = eButton::DpadUp;
	m_RegisteredDPad[down] = eButton::DpadDown;
}

void CInputManager::ActivateControllerUse()
{
	m_bControllerBeingUsed = true;
}

void CInputManager::DeactivateControllerUse()
{
	m_bControllerBeingUsed = false;
}
