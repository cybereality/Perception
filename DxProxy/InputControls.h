/*
* @file InputControls.h Holds the input controls class.
* @see InputControls
*/
#pragma once

#include <string>
#include <array>
#include <windows.h>
#include <WinUser.h>
#include <XInput.h>


/*
* Holds information about inputs.
*/
class InputControls
{
public:
	InputControls();
	/*
	* Refreshes the input status of the buttons and updates the input state.
	*/
	void UpdateXInputs();
	/*
	* Retrieves the key name for the given keycode
	* If the given keycode does not exists then it will return a hyphen
	*/
	std::string GetKeyName(int virtualKeyCode);
	/*
	* Sees if the key is down right now, takes into account xinput as well
	*/
	bool Key_Down(int virtualKeyCode);
	/*
	* Sees if the key is up right now, takes into account xinput as well
	*/
	bool Key_Up(int virtualKeyCode);
	/**
	* XInput controller button statuses.
	*/
	bool xButtonsStatus[16];
	/**
	* XInput controller state.
	**/
	XINPUT_STATE xInputState;
private:
	static std::array<std::string, 256> GetKeyNameList();
	/**
	* Virtual keys name list.
	* (used for BRASSA menu)
	**/
	static std::array<std::string, 256> KeyNameList;

};