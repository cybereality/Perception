#include "DirectXInputControls.h"
#include <assert.h>


DirectXInputControls::DirectXInputControls()
{
	Reset();
}

DirectXInputControls::~DirectXInputControls()
{
}

void DirectXInputControls::UpdateInputs()
{
	// Zeroise the XInput state
	ZeroMemory(&xInputState, sizeof(XINPUT_STATE));

	// Get the XInput state
	DWORD Result = XInputGetState(NULL, &xInputState);

	if(Result == ERROR_SUCCESS)
	{
		// set buttons by flags
		for(DWORD i = 0; i < 16;   ++i) 
			if((xInputState.Gamepad.wButtons >> i) & 1) 
				xButtonsStatus[i] = true;
			else 
				xButtonsStatus[i] = false;
	}
}

void DirectXInputControls::Reset()
{
	for (int i=0; i<16; i++)
		xButtonsStatus[i] = false;
}

bool DirectXInputControls::Key_Down(int virtualKeyCode)
{
	// FIXME: Don't hack the controller-handling this way
	return (GetAsyncKeyState(virtualKeyCode) & 0x8000) ? 1 : 0;
}

bool DirectXInputControls::Key_Up(int virtualKeyCode)
{
	return (GetAsyncKeyState(virtualKeyCode) & 0x8000) ? 0 : 1; ///TODO Should we be checking if xButtonStatus is false as well?
}

bool DirectXInputControls::GetButtonState(int button)
{
	return xButtonsStatus[button];
}

float DirectXInputControls::GetAxis(DirectXInputControls::GamepadAxis axis)
{
	int axisPosition;
	
	// Get axis position
	switch(axis) {
	case GamepadAxis::LeftStickX:  axisPosition = xInputState.Gamepad.sThumbLX; break;
	case GamepadAxis::LeftStickY:  axisPosition = xInputState.Gamepad.sThumbLY; break;
	case GamepadAxis::RightStickX: axisPosition = xInputState.Gamepad.sThumbRX; break;
	case GamepadAxis::RightStickY: axisPosition = xInputState.Gamepad.sThumbRY; break;
	default: assert(0); return 0;
	}
	
	// Normalize to [-1,1]
	return (float)axisPosition / 32768;
}
