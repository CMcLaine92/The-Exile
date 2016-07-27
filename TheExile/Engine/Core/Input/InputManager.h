/***********************************************
* Filename:  		InputManger.h
* Date:      		8/7/2015
* Mod. Date: 		8/17/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:   		Detects and manages keyboard,
*					mouse, and controller input
************************************************/

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H


#include <DirectXMath.h>

#include <unordered_map>

#include <Xinput.h>
#pragma comment(lib, "XInput9_1_0")

/* sensitivity values for controller:
RJ_X = 5000(max) -> 10000(Min)
RJ_Y = 6000(max) -> 11000(Min)

LJ_X = 
LJ_Y = 
*/

#define AXIS_DEADZONE 6500 //7849
#define DEADZONE_MAX 32767
#define AXIS_DEADZONE_LERP_X_VALUE 2000.0f //3925.0f //1880.0f //5969.0f
#define AXIS_DEADZONE_LERP_Y_VALUE 4000.0f
//#define AXIS_DEADZONE_LERP_MAX_VALUE (DEADZONE_MAX - AXIS_DEADZONE)
#define TRIGGER_THRESHOLD 75
#define CONTROLLER_CHECK_TIME 5.0f
#define NUMBER_OF_KEYS 256
#define CONTROLLER_AXIS_PRESSED_DELAY 0.15f
#define CONTROLLER_AXIS_HOLD_DELAY 0.3f
#define CONTROLLER_DC_TIMER 3.0f
#define CONTROLLER_SOFT_VIBRATE 35000
#define CONTROLLER_HARD_VIBRATE 65000 


//0-4, 8-9, 13, 16-18, 20, 27, 32, 37-40, 48-57, 65-90, 96-105, 112-123
//enum of keyCode constants for keyboard & mouse
enum eKey
{
	None = 0,

	LeftMouse = 1,
	RightMouse = 2,
	MiddleMouse = 4,

	Backspace = 8,
	Tab = 9,
	Enter = 13,
	Shift = 16,
	Control = 17,
	Alt = 18,
	CapsLock = 20,

	escape = 27,	
	Spacebar = 32,

	LeftArrow = 37,
	UpArrow = 38,
	RightArrow = 39,
	DownArrow = 40,

	Num0 = 48,
	Num1 = 49,
	Num2 = 50,
	Num3 = 51,
	Num4 = 52,
	Num5 = 53,
	Num6 = 54,
	Num7 = 55,
	Num8 = 56,
	Num9 = 57,

	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	NumPad0 = 96,
	NumPad1 = 97,
	NumPad2 = 98,
	NumPad3 = 99,
	NumPad4 = 100,
	NumPad5 = 101,
	NumPad6 = 102,
	NumPad7 = 103,
	NumPad8 = 104,
	NumPad9 = 105,

	F1 = 112,
	F2 = 113,
	F3 = 114,
	F4 = 115,
	F5 = 116,
	F6 = 117,
	F7 = 118,
	F8 = 119,
	F9 = 120,
	F10 = 121,
	F11 = 122,
	F12 = 123,

};

//xbox controlls
enum eButton
{
	NoButton = 0,
	AButton = XINPUT_GAMEPAD_A,
	BButton = XINPUT_GAMEPAD_B,
	YButton = XINPUT_GAMEPAD_Y,
	XButton = XINPUT_GAMEPAD_X,
	LeftBumper = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RightBumper = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	LeftThumbClick = XINPUT_GAMEPAD_LEFT_THUMB, 
	RightThumbClick = XINPUT_GAMEPAD_RIGHT_THUMB,
	Start = XINPUT_GAMEPAD_START,
	Back = XINPUT_GAMEPAD_BACK,
	DpadLeft = XINPUT_GAMEPAD_DPAD_LEFT,
	DpadRight = XINPUT_GAMEPAD_DPAD_RIGHT,
	DpadUp = XINPUT_GAMEPAD_DPAD_UP,
	DpadDown = XINPUT_GAMEPAD_DPAD_DOWN,
};

enum eAxis
{
	LeftX,
	LeftY,
	RightX,
	RightY,
};

enum eTriggers
{
	LeftTrigger,
	RightTrigger,
};

enum eButtonStates
{
	NONE = -1,
	PRESSED,
	DOWN,
};



class CInputManager
{
private:

	CInputManager() = default;
	virtual ~CInputManager() = default;

	static CInputManager* m_cpInstance;

	HWND m_InputWindow;

	bool m_bControllerBeingUsed;

#pragma region Keyboard and Mouse

	enum eBit_Key_States
	{
		Pressed_State = 128,
		Released_State = 64,
		Bit_Offset = 192,
	};

	unsigned char m_chKeyboard[NUMBER_OF_KEYS]; //keyboard/mouse key state array

	//int m_nScreenWidth = SCREEN_WIDTH;
	//int m_nScreenHeight = SCREEN_HEIGHT; // For screen wrap

	POINT m_ptMousePosition;
	XMFLOAT2 m_f2MouseDirection;

	unordered_map<string, vector<eKey>> m_RegisteredKeys;

#pragma endregion

#pragma region Controller

	struct TController
	{
		XINPUT_STATE m_currentState;
		XINPUT_STATE m_previousState;
		DWORD m_prevPacketNum;
		unsigned int m_unUserIndex;

		//vibration
		XINPUT_VIBRATION m_vibration;
		bool m_bVibrating;
		float m_fVibrateTime;
		unsigned int m_unLeftMotorSpeed;
		unsigned int m_unRightMotorSpeed;
	};

	TController m_tController;

	struct TInputAxis
	{
		eAxis m_eType;
		float m_nDir;
		eButtonStates m_eCurrentState;
		eButtonStates m_ePreviousState;
		float m_fAxisValue;
	};

	struct TTrigger
	{
		eTriggers m_eType;
		float m_fThreshHold;
		eButtonStates m_eCurrentState;
		eButtonStates m_ePreviousState;
	};

	unordered_map<string, vector<eButton>> m_RegisteredButtons;
	unordered_map<string, TInputAxis> m_RegisteredAxis;
	unordered_map<string, eButton> m_RegisteredDPad;
	unordered_map<string, TTrigger> m_RegisteredTriggers;
	bool m_bIsControllerConnected;
	float m_fControllerCheckTimer;
	bool m_bControllerDisconnected;

	bool m_bAxisLocked;
	float m_fAxisHoldDelayTimer;
	float m_fAxisPressedDelayTimer;


	/// <summary>
	/// Checks for conected controllers and sets the first one as the controller for use
	/// Returns true if successfully initialized.
	/// </summary>
	/// <param> none</param>
	bool CheckForControllers();

	/// <summary>
	/// Returns the axis value of the specified type
	/// </summary>
	/// <param name = "axisType"> The type of axis value to return.</param>
	short GetAxisValue(eAxis axisType) const;

#pragma endregion


	/// <summary>
	/// Initializes the InputManager.
	/// Returns true if successfully initialized.
	/// </summary>
	/// <param> none</param>
	bool Initialize();

	/// <summary>
	/// Shuts down the InputManager.
	/// Returns true is successfully shut down.
	/// </summary>
	/// <param> none</param>
	bool Shutdown();


public:

	/// <summary>
	/// Gets the current instance of the InputManager
	/// </summary>
	/// <param> none</param>
	static CInputManager* GetInstance();

	/// <summary>
	/// Deletes the current instance of the InputManager
	/// </summary>
	/// <param> none</param>
	static void DeleteInstance();

	/// <summary>
	/// Updates the InputManager.
	/// Returns true is there were no issues this update.
	/// </summary>
	/// <param> none</param>
	bool Update();

	/// <summary>
	/// Registers a name to 1 to 3 keys
	/// </summary>
	/// <param name = "name"> The name being registered to a key.</param>
	/// <param name = "key1"> The first key to be registed.</param>
	/// <param name = "key2"> The second key to be registed.</param>
	/// <param name = "key3"> The third key to be registed.</param>
	void RegisterKey(string name, eKey key1, eKey key2 = eKey::None, eKey key3 = eKey::None);

	/// <summary>
	/// Checks if the passed in name's registered key is pressed
	/// </summary>
	/// <param name = "name"> The name for the key being checked if pressed.</param>
	bool IsKeyPressed(string name);

	/// <summary>
	/// Checks if the passed in name's registered key is down
	/// </summary>
	/// <param name = "name"> The name for the key being checked if down.</param>
	bool IsKeyDown(string name);

	/// <summary>
	/// Registers a name to 1 to 3 buttons
	/// </summary>
	/// <param name = "name"> The name being registered to a button.</param>
	/// <param name = "button1"> The first button to be registed.</param>
	/// <param name = "button2"> The second button to be registed.</param>
	/// <param name = "button3"> The third button to be registed.</param>
	void RegisterButton(string name, eButton button1, eButton button2 = eButton::NoButton, eButton button3 = eButton::NoButton);

	/// Registers 2 names for a axis: one for positive values and one for negative values
	/// </summary>
	/// <param name = "negName"> The name being registered to a axis for the negative value.</param>
	/// <param name = "posName"> The name being registered to a axis for the positive value.</param>
	/// <param name = "axis"> The axis to be registed.</param>
	void RegisterAxis(string negName, string posName, eAxis axis);

	/// <summary>
	/// Checks if the passed in name's registered button is pressed
	/// </summary>
	/// <param name = "name"> The name for the button being checked if pressed.</param>
	bool IsButtonPressed(string name);

	/// <summary>
	/// Checks if the passed in name's registered button is down
	/// </summary>
	/// <param name = "name"> The name for the button being checked if down.</param>
	bool IsButtonDown(string name);

	/// <summary>
	/// Checks if the passed in name's registered Axes or DPad is down
	/// </summary>
	/// <param name = "name"> The name for the Axes or DPad being checked if down.</param>
	bool IsAxisDown(string name);

	/// <summary>
	/// Checks if the passed in name's registered Axes or DPad is pressed. It uses a delay timer. so that it can be held down but only activates pressed every ... sec
	/// </summary>
	/// <param name = "name"> The name for the Axes or DPad being checked if down.</param>
	bool IsAxisPressed(string name);

	/// <summary>
	/// Vibrates the controller 
	/// </summary>
	/// <param name = "leftMotor"> The left motor speed. Use 0-65535.</param>
	/// <param name = "rightMotor"> The right motor speed. Use 0-65535.</param>
	void VibrateController(unsigned int leftMotor, unsigned int rightMotor, float vibrateTime);

	void FlushKeys();

	void RegisterTrigger(string name, eTriggers trigger);
	bool IsTriggerDown(string name);
	bool IsTriggerPressed(string name);

	bool DidControllerDisconnect();
	bool IsControllerConnected();

	float GetRegisteredAxisValue(string name);

	/***************
	*  Accessors
	***************/
	/// <summary>
	/// Returns the Mouse movement direction
	/// </summary>
	/// <param> none</param>
	XMFLOAT2 GetMouseDirection() const;

	bool GetControllerBeingUsed() const;


	/***************
	* Mutators
	***************/

	/// <summary>
	/// Registers the 4 strings as the 4 DPad directions
	/// </summary>
	/// <param name = "left"> The name being registered to the left DPad direction.</param>
	/// <param name = "right"> The name being registered to the right DPad direction.</param>
	/// <param name = "up"> The name being registered to the up DPad direction.</param>
	/// <param name = "down"> The name being registered to the down DPad direction.</param>
	void SetDPad(string left, string right, string up, string down);

	void ActivateControllerUse();
	void DeactivateControllerUse();

};


#endif
