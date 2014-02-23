#include "DirectInput.h"

bool DirectInput::Init(HINSTANCE hinst, HWND hwnd)
{
	if(FAILED(DirectInput8Create(hinst, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&lpdi, NULL)))
	{
		return false;
	}
	lpdi->CreateDevice(GUID_SysMouse, &lpdimouse, NULL);
	lpdimouse->SetDataFormat(&c_dfDIMouse2);
	lpdimouse->SetCooperativeLevel(hwnd, 
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	lpdimouse->Acquire();
	return true;
}

void DirectInput::Shutdown()
{
	if(lpdimouse)
	{
		lpdimouse->Unacquire();
		lpdimouse->Release();
	}

	if(lpdi)
		lpdi->Release();
}

int DirectInput::ReadMouse()
{
	HRESULT hr;
	
	if(active == true && lpdimouse)	
	{
		hr = lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE2),
			&mousestate);
		if(FAILED(hr))
		{
			if(hr = DIERR_INPUTLOST)
			{
				hr = lpdimouse->Acquire();
				if(FAILED(hr))
				{
					if(hr==DIERR_OTHERAPPHASPRIO)
						return 2;
					else
						return 0;
				}
				lpdimouse->GetDeviceState(
					sizeof(DIMOUSESTATE2),
					&mousestate);
			}else
				return 0;
		}
	}
	return 1;
}

bool DirectInput::IsDown(int button)
{
	DirectInput::ReadMouse();
	return((mousestate.rgbButtons[button] & 0x80)!=0?true:false);
}
bool DirectInput::IsUp(int button)
{
	DirectInput::ReadMouse();
	return((mousestate.rgbButtons[button] & 0x80)!=0?false:true);
}

void DirectInput::GetCoords(int* x, int* y)
{
	DirectInput::ReadMouse();
	*x = mousestate.lX;
	*y = mousestate.lY;
}
int DirectInput::GetWheel()
{
	DirectInput::ReadMouse();
	return(mousestate.lZ);
}
void DirectInput::Activate()
{
	active = true;
}
void DirectInput::Deactivate()
{
	active = false;
}