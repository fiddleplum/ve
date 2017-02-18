#include "input_event.hpp"
#include "util/stringutil.hpp"
#include <SDL.h>

namespace ve
{
	InputEvent::InputEvent(int device, int axis, int value)
		: device(device), axis(axis), value(value)
	{
	}

	InputEvent::InputEvent(int device, int axis, std::string const & text)
		: device(device), axis(axis), text(text)
	{
	}

	int InputEvent::getDevice() const
	{
		return device;
	}

	int InputEvent::getAxis() const
	{
		return axis;
	}

	int InputEvent::getValue() const
	{
		return value;
	}

	std::string InputEvent::getText() const
	{
		return text;
	}

	bool InputEvent::isPressed() const
	{
		return value != 0;
	}

	bool InputEvent::isSameDeviceAndAxis(InputEvent const & inputEvent) const
	{
		return device == inputEvent.device && axis == inputEvent.axis;
	}

	std::string InputEvent::toString() const
	{
		std::string result;
		switch (device)
		{
			case DeviceKeyboard:
				result += "Keyboard ";
				if (axis == KeyboardText)
				{
					result += "Text " + text;
				}
				else
				{
					auto iter = keyToStringMap.find(axis);
					if (iter != keyToStringMap.end())
					{
						result += iter->second + (value == 0 ? " Up" : " Down");
					}
					else
					{
						result += "Unknown Key";
					}
				}
				break;
			case DeviceMouse:
				result += "Mouse ";
				switch (axis)
				{
					case MouseX: result += "X "; break;
					case MouseY: result += "Y "; break;
					case MouseWheel: result += "Wheel "; break;
					case MouseLeft: result += "Left "; break;
					case MouseMiddle: result += "Middle "; break;
					case MouseRight: result += "Right "; break;
					default: return "Button " + std::to_string(axis - MouseButton0) + " ";
				}
				switch (axis)
				{
					case MouseX: case MouseY: case MouseWheel:
						result += std::to_string(value); break;
					default:
						result += (value == 0 ? "Up" : "Down");
				}
				break;
			default: // Controllers
				result += "Controller " + (device - DeviceController0);
				if (axis < ControllerAxis0)
				{
					result += "Button " + std::to_string(axis - ControllerButton0) + " " + (value == 0 ? " Up" : " Down");
				}
				else
				{
					result += "Axis " + std::to_string(axis - ControllerAxis0) + " " + std::to_string(value);
				}
		}
		return result;
	}

	std::map<int, std::string> InputEvent::keyToStringMap = {
		{KeyboardA, "A"},
		{KeyboardB, "B"},
		{KeyboardC, "C"},
		{KeyboardD, "D"},
		{KeyboardE, "E"},
		{KeyboardF, "F"},
		{KeyboardG, "G"},
		{KeyboardH, "H"},
		{KeyboardI, "I"},
		{KeyboardJ, "J"},
		{KeyboardK, "K"},
		{KeyboardL, "L"},
		{KeyboardM, "M"},
		{KeyboardN, "N"},
		{KeyboardO, "O"},
		{KeyboardP, "P"},
		{KeyboardQ, "Q"},
		{KeyboardR, "R"},
		{KeyboardS, "S"},
		{KeyboardT, "T"},
		{KeyboardU, "U"},
		{KeyboardV, "V"},
		{KeyboardW, "W"},
		{KeyboardX, "X"},
		{KeyboardY, "Y"},
		{KeyboardZ, "Z"},
		{Keyboard0, "0"},
		{Keyboard1, "1"},
		{Keyboard2, "2"},
		{Keyboard3, "3"},
		{Keyboard4, "4"},
		{Keyboard5, "5"},
		{Keyboard6, "6"},
		{Keyboard7, "7"},
		{Keyboard8, "8"},
		{Keyboard9, "9"},
		{KeyboardUp, "Up"},
		{KeyboardDown, "Down"},
		{KeyboardLeft, "Left"},
		{KeyboardRight, "Right"},
		{KeyboardPageUp, "Page Up"},
		{KeyboardPageDown, "Page Down"},
		{KeyboardHome, "Home"},
		{KeyboardEnd, "End"},
		{KeyboardEnter, "Enter"},
		{KeyboardBackspace, "Backspace"},
		{KeyboardInsert, "Insert"},
		{KeyboardDelete, "Delete"},
		{KeyboardTab, "Tab"},
		{KeyboardSpace, "Space"},
		{KeyboardEscape, "Escape"},
		{KeyboardKeypad0, "Keypad 0"},
		{KeyboardKeypad1, "Keypad 1"},
		{KeyboardKeypad2, "Keypad 2"},
		{KeyboardKeypad3, "Keypad 3"},
		{KeyboardKeypad4, "Keypad 4"},
		{KeyboardKeypad5, "Keypad 5"},
		{KeyboardKeypad6, "Keypad 6"},
		{KeyboardKeypad7, "Keypad 7"},
		{KeyboardKeypad8, "Keypad 8"},
		{KeyboardKeypad9, "Keypad 9"},
		{KeyboardKeypadAdd, "Keypad Plus"},
		{KeyboardKeypadSubtract, "Keypad Minus"},
		{KeyboardKeypadMultiply, "Keypad Multiply"},
		{KeyboardKeypadDivide, "Keypad Divide"},
		{KeyboardKeypadDecimal, "Keypad Decimal"},
		{KeyboardKeypadEnter, "Keypad Enter"},
		{KeyboardGrave, "Grave"},
		{KeyboardApostrophe, "Apostrophe"},
		{KeyboardSemicolon, "Semicolon"},
		{KeyboardComma, "Comma"},
		{KeyboardPeriod, "Period"},
		{KeyboardSlash, "Slash"},
		{KeyboardLeftBracket, "Left Bracket"},
		{KeyboardRightBracket, "Right Bracket"},
		{KeyboardBackslash, "Backslash"},
		{KeyboardHyphen, "Hyphen"},
		{KeyboardEquals, "Equals"},
		{KeyboardCapslock, "Capslock"},
		{KeyboardLeftShift, "Left Shift"},
		{KeyboardRightShift, "Right Shift"},
		{KeyboardLeftControl, "Left Control"},
		{KeyboardRightControl, "Right Control"},
		{KeyboardLeftAlt, "Left Alt"},
		{KeyboardRightAlt, "Right Alt"},
		{KeyboardF1, "F1"},
		{KeyboardF2, "F2"},
		{KeyboardF3, "F3"},
		{KeyboardF4, "F4"},
		{KeyboardF5, "F5"},
		{KeyboardF6, "F6"},
		{KeyboardF7, "F7"},
		{KeyboardF8, "F8"},
		{KeyboardF9, "F9"},
		{KeyboardF10, "F10"},
		{KeyboardF11, "F11"},
		{KeyboardF12, "F12"},
		{KeyboardF13, "F13"},
		{KeyboardF14, "F14"},
		{KeyboardF15, "F15"},
		{KeyboardPause, "Pause"},
		{KeyboardText, "Text "}
	};
}