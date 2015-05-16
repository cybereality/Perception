/*
* @file InputControls.h Holds the input controls class.
* @see InputControls
*/
#pragma once

#include "json/json.h"
#include <string>
#include <array>
#include <memory>
#include <functional>
#include <vector>

class InputControls;
class InputControlState;

class InputBinding
{
public:
	virtual bool StateIsHeld(InputControlState *controlState)=0;
	
	bool IsHeld(InputControls &controls);
	bool IsPressed(InputControls &controls);
	bool IsReleased(InputControls &controls);
	
	virtual std::string ToString()=0;
	virtual Json::Value ToJson()=0;
};

typedef std::shared_ptr<InputBinding> InputBindingRef;

class UnboundKeyBinding
	: public InputBinding
{
public:
	bool StateIsHeld(InputControlState *state);
	std::string ToString();
	Json::Value ToJson();
};

class SimpleKeyBinding
	: public InputBinding
{
public:
	SimpleKeyBinding(int key);
	bool StateIsHeld(InputControlState *state);
	std::string ToString();
	Json::Value ToJson();
	int GetKeyCode() { return keyIndex; }
	
private:
	int keyIndex;
};

class SimpleButtonBinding
	: public InputBinding
{
public:
	SimpleButtonBinding(int button);
	bool StateIsHeld(InputControlState *state);
	std::string ToString();
	Json::Value ToJson();
	int GetButtonIndex() { return buttonIndex; }
	
private:
	int buttonIndex;
};

class CombinationKeyBinding
	: public InputBinding
{
public:
	CombinationKeyBinding(InputBindingRef first, InputBindingRef second);
	bool StateIsHeld(InputControlState *state);
	std::string ToString();
	Json::Value ToJson();
	InputBindingRef GetFirst();
	InputBindingRef GetSecond();
	
private:
	InputBindingRef first;
	InputBindingRef second;
};

class AlternativesKeyBinding
	: public InputBinding
{
public:
	AlternativesKeyBinding(InputBindingRef first, InputBindingRef second);
	bool StateIsHeld(InputControlState *state);
	std::string ToString();
	Json::Value ToJson();
	InputBindingRef GetFirst();
	InputBindingRef GetSecond();
	
private:
	InputBindingRef first;
	InputBindingRef second;
};

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
	
	virtual InputControlState *GetCurrentState()=0;
	virtual InputControlState *GetPreviousState()=0;
	
	enum GamepadAxis
	{
		LeftStickX,
		LeftStickY,
		RightStickX,
		RightStickY
	};
};

class InputControlState
{
public:
	/// Get a list of all held keys, buttons, and axes
	std::vector<InputBindingRef> GetHeldInputs();
	
	/// Whether a key is pressed
	virtual bool GetKeyState(int virtualKeyCode)=0;
	
	/// Whether a button is pressed
	virtual bool GetButtonState(int button)=0;
	
	/// Get the stick position of a gamepad axis, scaled from -1.0f to 1.0f.
	virtual float GetAxis(InputControls::GamepadAxis axis)=0;
};

class AxisThresholdBinding
	: public InputBinding
{
public:
	AxisThresholdBinding(InputControls::GamepadAxis axisIndex, bool greater, float threshold);
	bool StateIsHeld(InputControlState *controls);
	std::string ToString();
	Json::Value ToJson();
	
private:
	InputControls::GamepadAxis axisIndex;
	bool greater;
	float threshold;
};


namespace HotkeyExpressions
{
	InputBindingRef Key(int keyIndex);
	InputBindingRef Button(int buttonIndex);
	InputBindingRef Axis(InputControls::GamepadAxis axis, bool greater, float threshold);
	InputBindingRef operator+(InputBindingRef lhs, InputBindingRef rhs);
	InputBindingRef operator||(InputBindingRef lhs, InputBindingRef rhs);
	InputBindingRef Unbound();
	
	InputBindingRef HotkeyFromJson(const Json::Value &json);
	void UnpackAlternation(InputBindingRef hotkey, std::vector<InputBindingRef> *options);
	InputBindingRef PackAlternation(const std::vector<InputBindingRef> &options);
	InputBindingRef PackConjunction(const std::vector<InputBindingRef> &options);
}