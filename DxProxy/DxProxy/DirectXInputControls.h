#pragma once
#include <windows.h>
#include <WinUser.h>
#include <XInput.h>
#include "InputControls.h"
#include "ProxyHelper.h"

class DirectXInputControls;

class DirectXInputControlState
	:public InputControlState
{
public:
	DirectXInputControlState();
	void Reset();
	bool GetKeyState(int virtualKeyCode);
	bool GetButtonState(int button);
	float GetAxis(InputControls::GamepadAxis axis);
	
private:
	/// XInput controller button statuses.
	bool xButtonsStatus[16];
	
	/// XInput controller state.
	XINPUT_STATE xInputState;
	
	/// Whether each of 256 keys (indexed by VK) is held
	BYTE keyState[256];
	
	friend class DirectXInputControls;
};

class DirectXInputControls
	:public InputControls
{
public:
	DirectXInputControls(ProxyConfig *config);
	virtual ~DirectXInputControls();
	
	void UpdateInputs();
	void Reset();
	
	InputControlState *GetCurrentState();
	InputControlState *GetPreviousState();
	

private:
	ProxyConfig *config;
	
	DirectXInputControlState currentState;
	DirectXInputControlState prevState;
};
