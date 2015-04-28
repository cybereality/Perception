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

class InputBinding
{
public:
	virtual bool IsPressed(InputControls &controls)=0;
	virtual std::string ToString()=0;
	virtual Json::Value ToJson()=0;
};

typedef std::shared_ptr<InputBinding> InputBindingRef;

class UnboundKeyBinding
	: public InputBinding
{
public:
	bool IsPressed(InputControls &controls);
	std::string ToString();
	Json::Value ToJson();
};

class SimpleKeyBinding
	: public InputBinding
{
public:
	SimpleKeyBinding(int key);
	bool IsPressed(InputControls &controls);
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
	bool IsPressed(InputControls &controls);
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
	bool IsPressed(InputControls &controls);
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
	bool IsPressed(InputControls &controls);
	std::string ToString();
	Json::Value ToJson();
	InputBindingRef GetFirst();
	InputBindingRef GetSecond();
	
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
	
	InputBindingRef HotkeyFromJson(const Json::Value &json);
	void UnpackAlternation(InputBindingRef hotkey, std::vector<InputBindingRef> *options);
	InputBindingRef PackAlternation(const std::vector<InputBindingRef> &options);
	InputBindingRef PackConjunction(const std::vector<InputBindingRef> &options);
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
	
	std::vector<InputBindingRef> GetHeldInputs();
	
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
