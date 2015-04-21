#include "HotkeyExpressions.h"
#include "../DxProxy/DxProxy/InputControls.h"
#include "VireioUtil.h"

using namespace vireio;

InputBinding::~InputBinding()
{
}

bool UnboundKeyBinding::IsPressed(InputControls &controls)
{
	return false;
}

std::string UnboundKeyBinding::ToString()
{
	return "-";
}


SimpleKeyBinding::SimpleKeyBinding(int key)
{
	this->keyIndex = key;
}

bool SimpleKeyBinding::IsPressed(InputControls &controls)
{
	return controls.Key_Down(keyIndex);
}

std::string SimpleKeyBinding::ToString()
{
	if (keyIndex < KeyNameList.size() && keyIndex >= 0)
	{
		return KeyNameList[keyIndex];
	}
	else
	{
		return "-";
	}
}


SimpleButtonBinding::SimpleButtonBinding(int button)
{
	this->buttonIndex = button;
}

bool SimpleButtonBinding::IsPressed(InputControls &controls)
{
	return controls.xButtonsStatus[buttonIndex];
}

std::string SimpleButtonBinding::ToString()
{
	return retprintf("Btn%i", (int)buttonIndex);
}


InputExpressionBinding::InputExpressionBinding(std::string description, std::function<bool(InputControls&)> func)
{
	this->description = description;
	this->func = func;
}

bool InputExpressionBinding::IsPressed(InputControls &controls)
{
	return func(controls);
}

std::string InputExpressionBinding::ToString()
{
	return description;
}


CombinationKeyBinding::CombinationKeyBinding(InputBindingRef first, InputBindingRef second)
{
	this->first = first;
	this->second = second;
}

bool CombinationKeyBinding::IsPressed(InputControls &controls)
{
	return first->IsPressed(controls) && second->IsPressed(controls);
}

std::string CombinationKeyBinding::ToString()
{
	return retprintf("%s+%s",
		first->ToString().c_str(),
		second->ToString().c_str());
}


AlternativesKeyBinding::AlternativesKeyBinding(InputBindingRef first, InputBindingRef second)
{
	this->first = first;
	this->second = second;
}

bool AlternativesKeyBinding::IsPressed(InputControls &controls)
{
	return first->IsPressed(controls) || second->IsPressed(controls);
}

std::string AlternativesKeyBinding::ToString()
{
	return retprintf("%s or %s",
		first->ToString().c_str(),
		second->ToString().c_str());
}


InputBindingRef HotkeyExpressions::Key(int keyIndex)
{
	return InputBindingRef(new SimpleKeyBinding(keyIndex));
}

InputBindingRef HotkeyExpressions::Button(int buttonIndex)
{
	return InputBindingRef(new SimpleButtonBinding(buttonIndex));
}

InputBindingRef HotkeyExpressions::operator+(InputBindingRef lhs, InputBindingRef rhs)
{
	return InputBindingRef(new CombinationKeyBinding(lhs, rhs));
}

InputBindingRef HotkeyExpressions::operator||(InputBindingRef lhs, InputBindingRef rhs)
{
	return InputBindingRef(new AlternativesKeyBinding(lhs, rhs));
}
