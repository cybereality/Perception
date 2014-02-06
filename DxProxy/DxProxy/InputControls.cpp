#include "InputControls.h"

bool xButtonsStatus[16] = {false, false, false, false,
	false, false, false, false,
	false, false, false, false,
	false, false, false, false};

InputControls::InputControls()
{
	for (int i = 0; i < 16; i++ )
		xButtonsStatus[i] = false;
}

void InputControls::UpdateXInputs()
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

std::string InputControls::GetKeyName(int keyCode)
{
	if (keyCode < KeyNameList.size() && keyCode >= 0)
	{
		return KeyNameList[keyCode];
	}
	else
	{
		return "-";
	}
}

std::array<std::string, 256> InputControls::GetKeyNameList()
{
	std::array<std::string, 256> keyNameList;
	for (int i = 0; i < 256; i++)
		keyNameList[i] = "-";
	keyNameList[0x01] = "Left mouse button";
	keyNameList[0x02] = "Right mouse button";
	keyNameList[0x03] = "Control-break processing";
	keyNameList[0x04] = "Middle mouse button (three-button mouse)";
	keyNameList[0x08] = "BACKSPACE key";
	keyNameList[0x09] = "TAB key";
	keyNameList[0x0C] = "CLEAR key";
	keyNameList[0x0D] = "ENTER key";
	keyNameList[0x10] = "SHIFT key";
	keyNameList[0x11] = "CTRL key";
	keyNameList[0x12] = "ALT key";
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
	keyNameList[0xA0] = "Left SHIFT key";
	keyNameList[0xA1] = "Right SHIFT key";
	keyNameList[0xA2] = "Left CONTROL key";
	keyNameList[0xA3] = "Right CONTROL key";
	keyNameList[0xA4] = "Left MENU key";
	keyNameList[0xA5] = "Right MENU key";
	/// XInput hotkeys from 0xD0 to 0xDF
	keyNameList[0xD0] = "DPAD UP";
	keyNameList[0xD1] = "DPAD DOWN";
	keyNameList[0xD2] = "DPAD LEFT";
	keyNameList[0xD3] = "DPAD RIGHT";
	keyNameList[0xD4] = "START";
	keyNameList[0xD5] = "BACK";
	keyNameList[0xD6] = "LEFT THUMB";
	keyNameList[0xD7] = "RIGHT THUMB";
	keyNameList[0xD8] = "LEFT SHOULDER";
	keyNameList[0xD9] = "RIGHT SHOULDER";
	keyNameList[0xDC] = "Button A";
	keyNameList[0xDD] = "Button B";
	keyNameList[0xDE] = "Button X";
	keyNameList[0xDF] = "Button Y";
	/// end of XInput hotkeys
	keyNameList[0xFA] = "Play key";
	keyNameList[0xFB] = "Zoom key";
	return keyNameList;
}

bool InputControls::Key_Down( int virtualKeyCode )
{
	return (((GetAsyncKeyState(virtualKeyCode) & 0x8000) ? 1 : 0) || 
		((virtualKeyCode >= 0xD0) && (virtualKeyCode <= 0xDF) && (xButtonsStatus[virtualKeyCode % 0x10])));
}

bool InputControls::Key_Up( int virtualKeyCode )
{
	return ((GetAsyncKeyState(virtualKeyCode) & 0x8000) ? 0 : 1); ///TODO Should we be checking if xButtonStatus is false as well?
}

std::array<std::string, 256> InputControls::KeyNameList = InputControls::GetKeyNameList();