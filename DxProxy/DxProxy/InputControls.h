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
#include <memory>
#include <functional>

class InputControls;

class InputBinding
{
public:
	virtual bool IsPressed(InputControls &controls)=0;
};

typedef std::shared_ptr<InputBinding> InputBindingRef;

class SimpleKeyBinding
	: public InputBinding
{
public:
	SimpleKeyBinding(int key);
	bool IsPressed(InputControls &controls);
	
private:
	int keyIndex;
};

class SimpleButtonBinding
	: public InputBinding
{
public:
	SimpleButtonBinding(int button);
	bool IsPressed(InputControls &controls);
	
private:
	int buttonIndex;
};

class InputExpressionBinding
	: public InputBinding
{
public:
	InputExpressionBinding(std::function<bool(InputControls&)> func);
	bool IsPressed(InputControls &controls);

private:
	std::function<bool(InputControls&)> func;
};

class CombinationKeyBinding
	: public InputBinding
{
public:
	CombinationKeyBinding(InputBindingRef first, InputBindingRef second);
	bool IsPressed(InputControls &controls);
	
private:
	InputBindingRef first;
	InputBindingRef second;
};

class AlternativesKeyBinding
	: public InputBinding
{
public:
	AlternativesKeyBinding(InputBindingRef first, InputBindingRef second);
	bool IsPressed(InputControls &controls);
	
private:
	InputBindingRef first;
	InputBindingRef second;
};

namespace HotkeyExpressions
{
	InputBindingRef Key(int keyIndex);
	InputBindingRef Button(int buttonIndex);
	InputBindingRef operator+(InputBindingRef lhs, InputBindingRef rhs);
	InputBindingRef operator||(InputBindingRef lhs, InputBindingRef rhs);
}

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