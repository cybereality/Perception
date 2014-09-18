#pragma once

#ifndef DIRECTINPUT_H_INCLUDED
#define DIRECTINPUT_H_INCLUDED

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class DirectInput
{
public: 	
	bool active;	
	bool DirectInput::Init(HINSTANCE hinst, HWND hwnd);		
	bool DirectInput::IsDown(int button);
	bool DirectInput::IsUp(int button);
	void DirectInput::GetCoords(int* x, int* y);
	int DirectInput::GetWheel();
	void DirectInput::Activate();
	void DirectInput::Deactivate();
	void DirectInput::Shutdown();
private:
	int DirectInput::ReadMouse();	
	LPDIRECTINPUT8 lpdi;
	LPDIRECTINPUTDEVICE8 lpdimouse;
	DIMOUSESTATE2 mousestate;
};
#endif