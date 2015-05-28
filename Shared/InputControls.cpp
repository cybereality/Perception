#include "InputControls.h"
#include "VireioUtil.h"
#include "ConfigDefaults.h"
#include <assert.h>

using namespace vireio;

bool InputBinding::IsHeld(InputControls &controls)
{
	return StateIsHeld(controls.GetCurrentState());
}

bool InputBinding::IsPressed(InputControls &controls)
{
	return StateIsHeld(controls.GetCurrentState())
	   && !StateIsHeld(controls.GetPreviousState());
}

bool InputBinding::IsReleased(InputControls &controls)
{
	return !StateIsHeld(controls.GetCurrentState())
	     && StateIsHeld(controls.GetPreviousState());
}


static std::array<std::string, 256> GetKeyNameList()
{
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
	
	std::array<std::string, 256> keyNameList;
	for (int i = 0; i < 256; i++)
		keyNameList[i] = "-";
	keyNameList[0x01] = "Left mouse button";
	keyNameList[0x02] = "Right mouse button";
	keyNameList[0x03] = "Control-break processing";
	keyNameList[0x04] = "Middle mouse button (three-button mouse)";
	keyNameList[0x05] = "X1 mouse button";
	keyNameList[0x06] = "X2 mouse button";
	keyNameList[0x07] = "-";
	keyNameList[0x08] = "BACKSPACE key";
	keyNameList[0x09] = "TAB key";
	keyNameList[0x0C] = "CLEAR key";
	keyNameList[0x0D] = "ENTER key";
	keyNameList[0x0E] = "-";
	keyNameList[0x0F] = "-";
	// Don't assign key-names to the generic (unspecified whether left/right)
	// versions of ctrl, alt, shift. This prevents the silly issue binding a
	// key and it being "Shift+Left Shift+Key" with duplication instead of
	// "Left Shift+Key".
	//keyNameList[0x10] = "SHIFT key";
	//keyNameList[0x11] = "CTRL key";
	//keyNameList[0x12] = "ALT key";
	keyNameList[0x10] = "-";
	keyNameList[0x11] = "-";
	keyNameList[0x12] = "-";
	keyNameList[0x13] = "PAUSE key";
	keyNameList[0x14] = "CAPS LOCK key";
	keyNameList[0x1B] = "ESC key";
	keyNameList[0x20] = "SPACEBAR";
	keyNameList[0x21] = "PAGE UP key";
	keyNameList[0x22] = "PAGE DOWN key";
	keyNameList[0x23] = "END key";
	keyNameList[0x24] = "HOME key";
	keyNameList[0x25] = "LEFT ARROW key";
	keyNameList[0x26] = "UP ARROW key";
	keyNameList[0x27] = "RIGHT ARROW key";
	keyNameList[0x28] = "DOWN ARROW key";
	keyNameList[0x29] = "SELECT key";
	keyNameList[0x2A] = "PRINT key";
	keyNameList[0x2B] = "EXECUTE key";
	keyNameList[0x2C] = "PRINT SCREEN key";
	keyNameList[0x2D] = "INS key";
	keyNameList[0x2E] = "DEL key";
	keyNameList[0x2F] = "HELP key";
	keyNameList[0x30] = "0 key";
	keyNameList[0x31] = "1 key";
	keyNameList[0x32] = "2 key";
	keyNameList[0x33] = "3 key";
	keyNameList[0x34] = "4 key";
	keyNameList[0x35] = "5 key";
	keyNameList[0x36] = "6 key";
	keyNameList[0x37] = "7 key";
	keyNameList[0x38] = "8 key";
	keyNameList[0x39] = "9 key";
	keyNameList[0x41] = "A key";
	keyNameList[0x42] = "B key";
	keyNameList[0x43] = "C key";
	keyNameList[0x44] = "D key";
	keyNameList[0x45] = "E key";
	keyNameList[0x46] = "F key";
	keyNameList[0x47] = "G key";
	keyNameList[0x48] = "H key";
	keyNameList[0x49] = "I key";
	keyNameList[0x4A] = "J key";
	keyNameList[0x4B] = "K key";
	keyNameList[0x4C] = "L key";
	keyNameList[0x4D] = "M key";
	keyNameList[0x4E] = "N key";
	keyNameList[0x4F] = "O key";
	keyNameList[0x50] = "P key";
	keyNameList[0x51] = "Q key";
	keyNameList[0x52] = "R key";
	keyNameList[0x53] = "S key";
	keyNameList[0x54] = "T key";
	keyNameList[0x55] = "U key";
	keyNameList[0x56] = "V key";
	keyNameList[0x57] = "W key";
	keyNameList[0x58] = "X key";
	keyNameList[0x59] = "Y key";
	keyNameList[0x5A] = "Z key";
	keyNameList[0x60] = "Numeric keypad 0 key";
	keyNameList[0x61] = "Numeric keypad 1 key";
	keyNameList[0x62] = "Numeric keypad 2 key";
	keyNameList[0x63] = "Numeric keypad 3 key";
	keyNameList[0x64] = "Numeric keypad 4 key";
	keyNameList[0x65] = "Numeric keypad 5 key";
	keyNameList[0x66] = "Numeric keypad 6 key";
	keyNameList[0x67] = "Numeric keypad 7 key";
	keyNameList[0x68] = "Numeric keypad 8 key";
	keyNameList[0x69] = "Numeric keypad 9 key";
	keyNameList[0x6C] = "Separator key";
	keyNameList[0x6D] = "Subtract key";
	keyNameList[0x6E] = "Decimal key";
	keyNameList[0x6F] = "Divide key";
	keyNameList[0x70] = "F1 key";
	keyNameList[0x71] = "F2 key";
	keyNameList[0x72] = "F3 key";
	keyNameList[0x73] = "F4 key";
	keyNameList[0x74] = "F5 key";
	keyNameList[0x75] = "F6 key";
	keyNameList[0x76] = "F7 key";
	keyNameList[0x77] = "F8 key";
	keyNameList[0x78] = "F9 key";
	keyNameList[0x79] = "F10 key";
	keyNameList[0x7A] = "F11 key";
	keyNameList[0x7B] = "F12 key";
	keyNameList[0x7C] = "F13 key";
	keyNameList[0x7D] = "F14 key";
	keyNameList[0x7E] = "F15 key";
	keyNameList[0x7F] = "F16 key";
	keyNameList[0x80] = "F17 key";
	keyNameList[0x81] = "F18 key";
	keyNameList[0x82] = "F19 key";
	keyNameList[0x83] = "F20 key";
	keyNameList[0x84] = "F21 key";
	keyNameList[0x85] = "F22 key";
	keyNameList[0x86] = "F23 key";
	keyNameList[0x87] = "F24 key";
	keyNameList[0x90] = "NUM LOCK key";
	keyNameList[0x91] = "SCROLL LOCK key";
	keyNameList[0xA0] = "Left SHIFT";
	keyNameList[0xA1] = "Right SHIFT";
	keyNameList[0xA2] = "Left CONTROL";
	keyNameList[0xA3] = "Right CONTROL";
	keyNameList[0xA4] = "Left MENU";
	keyNameList[0xA5] = "Right MENU";
	keyNameList[0xD0] = "-";
	keyNameList[0xD1] = "-";
	keyNameList[0xD2] = "-";
	keyNameList[0xD3] = "-";
	keyNameList[0xD4] = "-";
	keyNameList[0xD5] = "-";
	keyNameList[0xD6] = "-";
	keyNameList[0xD7] = "-";
	keyNameList[0xD8] = "{[ key";
	keyNameList[0xD9] = "-";
	keyNameList[0xDA] = "-";
	keyNameList[0xDB] = "-";
	keyNameList[0xDC] = "\\ key";
	keyNameList[0xDD] = "}] key";
	keyNameList[0xDE] = "'\" key";
	keyNameList[0xDF] = "-";
	/// end of XInput hotkeys
	keyNameList[0xFA] = "Play key";
	keyNameList[0xFB] = "Zoom key";
	return keyNameList;
}

static std::array<std::string, 16> GetButtonNameList()
{
	std::array<std::string, 16> buttonNameList;
	buttonNameList[0] = "DPAD UP";
	buttonNameList[1] = "DPAD DOWN";
	buttonNameList[2] = "DPAD LEFT";
	buttonNameList[3] = "DPAD RIGHT";
	buttonNameList[4] = "START";
	buttonNameList[5] = "BACK";
	buttonNameList[6] = "LEFT THUMB";
	buttonNameList[7] = "RIGHT THUMB";
	buttonNameList[8] = "LEFT SHOULDER";
	buttonNameList[9] = "RIGHT SHOULDER";
	//10
	//11
	buttonNameList[12] = "Button A";
	buttonNameList[13] = "Button B";
	buttonNameList[14] = "Button X";
	buttonNameList[15] = "Button Y";
	return buttonNameList;
}

static std::vector<InputBindingRef> GetBindableAxes()
{
	std::vector<InputBindingRef> ret;
	ret.push_back(HotkeyExpressions::Axis(InputControls::LeftStickX, true, BOUND_STICK_DEADZONE));
	ret.push_back(HotkeyExpressions::Axis(InputControls::LeftStickX, false, -BOUND_STICK_DEADZONE));
	ret.push_back(HotkeyExpressions::Axis(InputControls::LeftStickY, true, BOUND_STICK_DEADZONE));
	ret.push_back(HotkeyExpressions::Axis(InputControls::LeftStickY, false, -BOUND_STICK_DEADZONE));
	ret.push_back(HotkeyExpressions::Axis(InputControls::RightStickX, true, BOUND_STICK_DEADZONE));
	ret.push_back(HotkeyExpressions::Axis(InputControls::RightStickX, false, -BOUND_STICK_DEADZONE));
	ret.push_back(HotkeyExpressions::Axis(InputControls::RightStickY, true, BOUND_STICK_DEADZONE));
	ret.push_back(HotkeyExpressions::Axis(InputControls::RightStickY, false, -BOUND_STICK_DEADZONE));
	return ret;
}

// Virtual keys name list (used for BRASSA menu)
static std::array<std::string, 256> KeyNameList = GetKeyNameList();
static std::array<std::string, 16> ButtonNameList = GetButtonNameList();

// List of bind-able axes
static std::vector<InputBindingRef> BindableAxes = GetBindableAxes();


InputControls::InputControls()
{
}

InputControls::~InputControls()
{
}

std::vector<InputBindingRef> InputControlState::GetHeldInputs()
{
	std::vector<InputBindingRef> ret;
	
	for(int ii=0; ii<256; ii++)
	{
		if(GetKeyState(ii) && HotkeyExpressions::Key(ii)->ToString()!="-")
		{
			ret.push_back(HotkeyExpressions::Key(ii));
		}
	}
	
	for(int ii=0; ii<16; ii++)
	{
		if(GetButtonState(ii))
		{
			ret.push_back(HotkeyExpressions::Button(ii));
		}
	}
	
	for(size_t ii=0; ii<BindableAxes.size(); ii++)
	{
		if(BindableAxes[ii]->StateIsHeld(this))
		{
			ret.push_back(BindableAxes[ii]);
		}
	}
	
	return ret;
}


bool UnboundKeyBinding::StateIsHeld(InputControlState *state)
{
	return false;
}

std::string UnboundKeyBinding::ToString()
{
	return "-";
}

Json::Value UnboundKeyBinding::ToJson()
{
	Json::Value arr = Json::Value(Json::arrayValue);
	arr.append("unbound");
	return arr;
}


SimpleKeyBinding::SimpleKeyBinding(int key)
{
	this->keyIndex = key;
}

bool SimpleKeyBinding::StateIsHeld(InputControlState *state)
{
	return state->GetKeyState(keyIndex);
}

std::string SimpleKeyBinding::ToString()
{
	if (keyIndex < (int)KeyNameList.size() && keyIndex >= 0)
	{
		return KeyNameList[keyIndex];
	}
	else
	{
		return "-";
	}
}

Json::Value SimpleKeyBinding::ToJson()
{
	Json::Value arr = Json::Value(Json::arrayValue);
	arr.append("key");
	arr.append(keyIndex);
	return arr;
}


SimpleButtonBinding::SimpleButtonBinding(int button)
{
	this->buttonIndex = button;
}

bool SimpleButtonBinding::StateIsHeld(InputControlState *state)
{
	return state->GetButtonState(buttonIndex);
}

std::string SimpleButtonBinding::ToString()
{
	return retprintf("Btn%i", (int)buttonIndex);
}

Json::Value SimpleButtonBinding::ToJson()
{
	Json::Value arr = Json::Value(Json::arrayValue);
	arr.append("button");
	arr.append(buttonIndex);
	return arr;
}


CombinationKeyBinding::CombinationKeyBinding(InputBindingRef first, InputBindingRef second)
{
	this->first = first;
	this->second = second;
}

bool CombinationKeyBinding::StateIsHeld(InputControlState *state)
{
	return first->StateIsHeld(state) && second->StateIsHeld(state);
}

std::string CombinationKeyBinding::ToString()
{
	return retprintf("%s+%s",
		first->ToString().c_str(),
		second->ToString().c_str());
}

Json::Value CombinationKeyBinding::ToJson()
{
	Json::Value arr = Json::Value(Json::arrayValue);
	arr.append("and");
	arr.append(first->ToJson());
	arr.append(second->ToJson());
	return arr;
}

InputBindingRef CombinationKeyBinding::GetFirst()
{
	return first;
}

InputBindingRef CombinationKeyBinding::GetSecond()
{
	return second;
}


AlternativesKeyBinding::AlternativesKeyBinding(InputBindingRef first, InputBindingRef second)
{
	this->first = first;
	this->second = second;
}

bool AlternativesKeyBinding::StateIsHeld(InputControlState *state)
{
	return first->StateIsHeld(state) || second->StateIsHeld(state);
}

std::string AlternativesKeyBinding::ToString()
{
	return retprintf("%s or %s",
		first->ToString().c_str(),
		second->ToString().c_str());
}

Json::Value AlternativesKeyBinding::ToJson()
{
	Json::Value arr = Json::Value(Json::arrayValue);
	arr.append("or");
	arr.append(first->ToJson());
	arr.append(second->ToJson());
	return arr;
}

InputBindingRef AlternativesKeyBinding::GetFirst()
{
	return first;
}

InputBindingRef AlternativesKeyBinding::GetSecond()
{
	return second;
}


AxisThresholdBinding::AxisThresholdBinding(InputControls::GamepadAxis axisIndex, bool greater, float threshold)
{
	this->axisIndex = axisIndex;
	this->greater = greater;
	this->threshold = threshold;
}

bool AxisThresholdBinding::StateIsHeld(InputControlState *state)
{
	float axisPosition = state->GetAxis(axisIndex);
	
	if(greater) {
		return axisPosition>threshold;
	} else {
		return axisPosition<threshold;
	}
}

std::string AxisThresholdBinding::ToString()
{
	switch(axisIndex)
	{
	case InputControls::GamepadAxis::LeftStickX:
		if(greater) return "LThumb Right";
		else        return "LThumb Left";
	case InputControls::GamepadAxis::LeftStickY:
		if(greater) return "LThumb Up";
		else        return "LThumb Down";
	case InputControls::GamepadAxis::RightStickX:
		if(greater) return "RThumb Right";
		else        return "RThumb Left";
	case InputControls::GamepadAxis::RightStickY:
		if(greater) return "RThumb Up";
		else        return "RThumb Down";
	default:
		return "???";
	}
}

Json::Value AxisThresholdBinding::ToJson()
{
	Json::Value arr = Json::Value(Json::arrayValue);
	arr.append("axis");
	arr.append((int)axisIndex);
	arr.append(greater);
	arr.append(threshold);
	return arr;
}


HotkeyRemapping::HotkeyRemapping()
{
	Key = HotkeyExpressions::Unbound();
	ExcludeKey = HotkeyExpressions::Unbound();
	BoundTo = 0;
	IsHeld = false;
}

HotkeyRemapping::HotkeyRemapping(Json::Value json)
{
	if(!json.isObject())
		throw std::exception("Malformed JSON: HotkeyRemapping should be an object");
	
	Key = HotkeyExpressions::HotkeyFromJson(json["key"]);
	ExcludeKey = HotkeyExpressions::HotkeyFromJson(json["exclude"]);
	BoundTo = json["boundTo"].asInt();
}

std::string HotkeyRemapping::ToString()
{
	return retprintf("%s => %s",
		Key->ToString().c_str(),
		HotkeyExpressions::Key(BoundTo)->ToString().c_str());
}

Json::Value HotkeyRemapping::ToJson()
{
	Json::Value obj = Json::Value(Json::objectValue);
	obj["key"] = Key->ToJson();
	obj["exclude"] = ExcludeKey->ToJson();
	obj["boundTo"] = (int)BoundTo;
	return obj;
}


HotkeyRemappingSet::HotkeyRemappingSet()
{
}

HotkeyRemappingSet::HotkeyRemappingSet(Json::Value json)
{
	if(!json.isArray()) {
		throw std::exception("Malformed JSON: HotkeyRemappingSet should be an array");
	}
	
	for(size_t ii=0; ii<json.size(); ii++)
		Remappings.push_back(HotkeyRemapping(json[ii]));
}

Json::Value HotkeyRemappingSet::ToJson()
{
	Json::Value arr = Json::Value(Json::arrayValue);
	for(size_t ii=0; ii<Remappings.size(); ii++)
	{
		arr.append(Remappings[ii].ToJson());
	}
	return arr;
}


InputBindingRef HotkeyExpressions::Key(int keyIndex)
{
	return InputBindingRef(new SimpleKeyBinding(keyIndex));
}

InputBindingRef HotkeyExpressions::Button(int buttonIndex)
{
	return InputBindingRef(new SimpleButtonBinding(buttonIndex));
}

InputBindingRef HotkeyExpressions::Axis(InputControls::GamepadAxis axis, bool greater, float threshold)
{
	return InputBindingRef(new AxisThresholdBinding(axis, greater, threshold));
}

InputBindingRef HotkeyExpressions::operator+(InputBindingRef lhs, InputBindingRef rhs)
{
	return InputBindingRef(new CombinationKeyBinding(lhs, rhs));
}

InputBindingRef HotkeyExpressions::operator||(InputBindingRef lhs, InputBindingRef rhs)
{
	return InputBindingRef(new AlternativesKeyBinding(lhs, rhs));
}

InputBindingRef HotkeyExpressions::Unbound()
{
	return InputBindingRef(new UnboundKeyBinding());
}

void HotkeyExpressions::UnpackAlternation(InputBindingRef hotkey, std::vector<InputBindingRef> *options)
{
	AlternativesKeyBinding *alternatives = dynamic_cast<AlternativesKeyBinding*>(&*hotkey);
	UnboundKeyBinding *unbound = dynamic_cast<UnboundKeyBinding*>(&*hotkey);
	if(alternatives != NULL) {
		UnpackAlternation(alternatives->GetFirst(), options);
		UnpackAlternation(alternatives->GetSecond(), options);
	} else if(unbound != NULL) {
		return;
	} else {
		options->push_back(hotkey);
	}
}

InputBindingRef HotkeyExpressions::PackAlternation(const std::vector<InputBindingRef> &options)
{
	if(options.size() == 0)
		return Unbound();
	InputBindingRef result = options[0];
	for(size_t ii=1; ii<options.size(); ii++)
		result = result || options[ii];
	return result;
}

InputBindingRef HotkeyExpressions::PackConjunction(const std::vector<InputBindingRef> &options)
{
	if(options.size() == 0)
		return Unbound();
	InputBindingRef result = options[0];
	for(size_t ii=1; ii<options.size(); ii++)
		result = result + options[ii];
	return result;
}

InputBindingRef HotkeyExpressions::HotkeyFromJson(const Json::Value &json)
{
	if(json.isInt()) {
		// Bare number is a keycode
		return Key(json.asInt());
	} else if(json.isArray()) {
		// Array: First element is type, remainder is value
		if(json.size() < 1) return Unbound();
		std::string type = json[0].asString();
		if(type == "key") {
			if(json.size() < 2) return Unbound();
			int keyCode = json[1].asInt();
			return Key(keyCode);
		} else if(type == "button") {
			if(json.size() < 2) return Unbound();
			int buttonIndex = json[1].asInt();
			return Button(buttonIndex);
		} else if(type == "axis") {
			if(json.size() < 4) return Unbound();
			int axisIndex = json[1].asInt();
			bool isGreater = json[2].asBool();
			float axisThreshold = json[3].asFloat();
			return Axis((InputControls::GamepadAxis)axisIndex, isGreater, axisThreshold);
		} else if(type == "and") {
			if(json.size() < 3) return Unbound();
			InputBindingRef first = HotkeyFromJson(json[1]);
			InputBindingRef second = HotkeyFromJson(json[2]);
			return first+second;
		} else if(type == "or") {
			if(json.size() < 3) return Unbound();
			InputBindingRef first = HotkeyFromJson(json[1]);
			InputBindingRef second = HotkeyFromJson(json[2]);
			return first||second;
		} else {
			return Unbound();
		}
	} else {
		return Unbound();
	}
}
