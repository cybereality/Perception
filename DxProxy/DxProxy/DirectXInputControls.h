#pragma once
#include <windows.h>
#include <WinUser.h>
#include <XInput.h>
#include "InputControls.h"

class DirectXInputControls
	:public InputControls
{
public:
	DirectXInputControls();
	virtual ~DirectXInputControls();
	
	void UpdateInputs();
	void Reset();
	std::string GetKeyName(int virtualKeyCode);
	bool Key_Down(int virtualKeyCode);
	bool Key_Up(int virtualKeyCode);
	bool GetButtonState(int button);
	float GetAxis(GamepadAxis axis);
	

private:
	/// XInput controller button statuses.
	bool xButtonsStatus[16];
	
	/// XInput controller state.
	XINPUT_STATE xInputState;
};