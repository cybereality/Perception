/*
* @file InputControls.h Holds the input controls class.
* @see InputControls
*/
#pragma once

#include <string>
#include <array>
#include <memory>
#include <functional>

class InputControls;

class InputBinding
{
public:
	virtual bool IsPressed(InputControls &controls)=0;
	virtual std::string ToString()=0;
};

typedef std::shared_ptr<InputBinding> InputBindingRef;

class UnboundKeyBinding
	: public InputBinding
{
public:
	bool IsPressed(InputControls &controls);
	std::string ToString();
};

class SimpleKeyBinding
	: public InputBinding
{
public:
	SimpleKeyBinding(int key);
	bool IsPressed(InputControls &controls);
	std::string ToString();
	int GetKeyCode() { return keyIndex; }
	
private:
	int keyIndex;
};

class SimpleButtonBinding
	: public InputBinding
{
public:
	SimpleButtonBinding(int button);
	bool IsPressed(InputControls &controls);
	std::string ToString();
	int GetButtonIndex() { return buttonIndex; }
	
private:
	int buttonIndex;
};

class InputExpressionBinding
	: public InputBinding
{
public:
	InputExpressionBinding(std::string description, std::function<bool(InputControls&)> func);
	bool IsPressed(InputControls &controls);
	std::string ToString();

private:
	std::string description;
	std::function<bool(InputControls&)> func;
};

class CombinationKeyBinding
	: public InputBinding
{
public:
	CombinationKeyBinding(InputBindingRef first, InputBindingRef second);
	bool IsPressed(InputControls &controls);
	std::string ToString();
	
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
	std::string ToString();
	
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
	InputBindingRef Unbound();
}

/*
* Holds information about inputs.
*/
class InputControls
{
public:
	InputControls();
	virtual ~InputControls();
	
	/// Refreshes the input status of the buttons and updates the input state.
	virtual void UpdateInputs()=0;
	virtual void Reset()=0;
	
	/// Sees if the key is down right now, takes into account xinput as well
	virtual bool Key_Down(int virtualKeyCode)=0;
	
	/// Sees if the key is up right now, takes into account xinput as well
	virtual bool Key_Up(int virtualKeyCode)=0;
	
	virtual bool GetButtonState(int button)=0;
	
	enum GamepadAxis
	{
		LeftStickX,
		LeftStickY,
		RightStickX,
		RightStickY
	};
	
	/// Get the stick position of a gamepad axis, scaled from -1.0f to 1.0f.
	virtual float GetAxis(GamepadAxis axis)=0;
};
