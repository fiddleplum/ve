#include "input_event.hpp"
#include "util/stringutil.hpp"
#include <SDL.h>

namespace ve
{
	Input::Event::Event(int device, int axis, int value)
		: device(device), axis(axis), value(value)
	{
	}

	Input::Event::Event(int device, int axis, std::string const & text)
		: device(device), axis(axis), text(text)
	{
	}

	int Input::Event::getDevice() const
	{
		return device;
	}

	int Input::Event::getAxis() const
	{
		return axis;
	}

	int Input::Event::getValue() const
	{
		return value;
	}

	std::string Input::Event::getText() const
	{
		return text;
	}

	bool Input::Event::isPressed() const
	{
		return value != 0;
	}

	bool Input::Event::isSameDeviceAndAxis(Input::Event const & inputEvent) const
	{
		return device == inputEvent.device && axis == inputEvent.axis;
	}

	std::string Input::Event::toString() const
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

	void Input::populateFromSDLEvents(std::vector<Input::Event> & events, SDL_Event const & sdlEvent)
	{
		int axis;
		int value;
		std::string text;
		auto iter = sdlKeyToKeyMap.begin();
		switch (sdlEvent.type)
		{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				iter = sdlKeyToKeyMap.find(sdlEvent.key.keysym.sym);
				if (iter != sdlKeyToKeyMap.end())
				{
					axis = iter->second;
					value = (sdlEvent.type == SDL_KEYDOWN ? 1 : 0);
					events.push_back(Event(DeviceKeyboard, axis, value));
				}
				break;
			case SDL_TEXTINPUT:
				axis = KeyboardText;
				text = sdlEvent.text.text;
				events.push_back(Event(DeviceKeyboard, axis, text));
				break;
			case SDL_MOUSEMOTION:
				axis = MouseX;
				value = sdlEvent.motion.xrel;
				events.push_back(Event(DeviceMouse, axis, value));
				axis = MouseY;
				value = sdlEvent.motion.yrel;
				events.push_back(Event(DeviceMouse, axis, value));
				break;
			case SDL_MOUSEWHEEL:
				axis = MouseWheel;
				value = (sdlEvent.wheel.y * (sdlEvent.wheel.direction == SDL_MOUSEWHEEL_NORMAL ? 1 : -1) > 0);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				switch (sdlEvent.button.button)
				{
					case SDL_BUTTON_LEFT: axis = MouseLeft; break;
					case SDL_BUTTON_MIDDLE: axis = MouseMiddle; break;
					case SDL_BUTTON_RIGHT: axis = MouseRight; break;
					case SDL_BUTTON_X1: axis = MouseButton0 + 3; break;
					case SDL_BUTTON_X2: axis = MouseButton0 + 4; break;
				}
				value = (sdlEvent.type == SDL_MOUSEBUTTONDOWN ? 1 : 0);
				events.push_back(Event(DeviceMouse, axis, value));
				break;
		}
	}

	void Input::enableTextInput(bool enable)
	{
		if (enable)
		{
			SDL_StartTextInput();
		}
		else
		{
			SDL_StopTextInput();
		}
	}

	std::map<int, std::string> Input::Event::keyToStringMap = {
		{KeyboardA, "A"},
		{Input::KeyboardB, "B"},
		{Input::KeyboardC, "C"},
		{Input::KeyboardD, "D"},
		{Input::KeyboardE, "E"},
		{Input::KeyboardF, "F"},
		{Input::KeyboardG, "G"},
		{Input::KeyboardH, "H"},
		{Input::KeyboardI, "I"},
		{Input::KeyboardJ, "J"},
		{Input::KeyboardK, "K"},
		{Input::KeyboardL, "L"},
		{Input::KeyboardM, "M"},
		{Input::KeyboardN, "N"},
		{Input::KeyboardO, "O"},
		{Input::KeyboardP, "P"},
		{Input::KeyboardQ, "Q"},
		{Input::KeyboardR, "R"},
		{Input::KeyboardS, "S"},
		{Input::KeyboardT, "T"},
		{Input::KeyboardU, "U"},
		{Input::KeyboardV, "V"},
		{Input::KeyboardW, "W"},
		{Input::KeyboardX, "X"},
		{Input::KeyboardY, "Y"},
		{Input::KeyboardZ, "Z"},
		{Input::Keyboard0, "0"},
		{Input::Keyboard1, "1"},
		{Input::Keyboard2, "2"},
		{Input::Keyboard3, "3"},
		{Input::Keyboard4, "4"},
		{Input::Keyboard5, "5"},
		{Input::Keyboard6, "6"},
		{Input::Keyboard7, "7"},
		{Input::Keyboard8, "8"},
		{Input::Keyboard9, "9"},
		{Input::KeyboardUp, "Up"},
		{Input::KeyboardDown, "Down"},
		{Input::KeyboardLeft, "Left"},
		{Input::KeyboardRight, "Right"},
		{Input::KeyboardPageUp, "Page Up"},
		{Input::KeyboardPageDown, "Page Down"},
		{Input::KeyboardHome, "Home"},
		{Input::KeyboardEnd, "End"},
		{Input::KeyboardEnter, "Enter"},
		{Input::KeyboardBackspace, "Backspace"},
		{Input::KeyboardInsert, "Insert"},
		{Input::KeyboardDelete, "Delete"},
		{Input::KeyboardTab, "Tab"},
		{Input::KeyboardSpace, "Space"},
		{Input::KeyboardEscape, "Escape"},
		{Input::KeyboardKeypad0, "Keypad 0"},
		{Input::KeyboardKeypad1, "Keypad 1"},
		{Input::KeyboardKeypad2, "Keypad 2"},
		{Input::KeyboardKeypad3, "Keypad 3"},
		{Input::KeyboardKeypad4, "Keypad 4"},
		{Input::KeyboardKeypad5, "Keypad 5"},
		{Input::KeyboardKeypad6, "Keypad 6"},
		{Input::KeyboardKeypad7, "Keypad 7"},
		{Input::KeyboardKeypad8, "Keypad 8"},
		{Input::KeyboardKeypad9, "Keypad 9"},
		{Input::KeyboardKeypadAdd, "Keypad Plus"},
		{Input::KeyboardKeypadSubtract, "Keypad Minus"},
		{Input::KeyboardKeypadMultiply, "Keypad Multiply"},
		{Input::KeyboardKeypadDivide, "Keypad Divide"},
		{Input::KeyboardKeypadDecimal, "Keypad Decimal"},
		{Input::KeyboardKeypadEnter, "Keypad Enter"},
		{Input::KeyboardGrave, "Grave"},
		{Input::KeyboardApostrophe, "Apostrophe"},
		{Input::KeyboardSemicolon, "Semicolon"},
		{Input::KeyboardComma, "Comma"},
		{Input::KeyboardPeriod, "Period"},
		{Input::KeyboardSlash, "Slash"},
		{Input::KeyboardLeftBracket, "Left Bracket"},
		{Input::KeyboardRightBracket, "Right Bracket"},
		{Input::KeyboardBackslash, "Backslash"},
		{Input::KeyboardHyphen, "Hyphen"},
		{Input::KeyboardEquals, "Equals"},
		{Input::KeyboardCapslock, "Capslock"},
		{Input::KeyboardLeftShift, "Left Shift"},
		{Input::KeyboardRightShift, "Right Shift"},
		{Input::KeyboardLeftControl, "Left Control"},
		{Input::KeyboardRightControl, "Right Control"},
		{Input::KeyboardLeftAlt, "Left Alt"},
		{Input::KeyboardRightAlt, "Right Alt"},
		{Input::KeyboardF1, "F1"},
		{Input::KeyboardF2, "F2"},
		{Input::KeyboardF3, "F3"},
		{Input::KeyboardF4, "F4"},
		{Input::KeyboardF5, "F5"},
		{Input::KeyboardF6, "F6"},
		{Input::KeyboardF7, "F7"},
		{Input::KeyboardF8, "F8"},
		{Input::KeyboardF9, "F9"},
		{Input::KeyboardF10, "F10"},
		{Input::KeyboardF11, "F11"},
		{Input::KeyboardF12, "F12"},
		{Input::KeyboardF13, "F13"},
		{Input::KeyboardF14, "F14"},
		{Input::KeyboardF15, "F15"},
		{Input::KeyboardPause, "Pause"},
		{Input::KeyboardText, "Text "}
	};

	std::map<int, int> Input::sdlKeyToKeyMap = {
		{SDLK_a, KeyboardA},
		{SDLK_b, KeyboardB},
		{SDLK_c, KeyboardC},
		{SDLK_d, KeyboardD},
		{SDLK_e, KeyboardE},
		{SDLK_f, KeyboardF},
		{SDLK_g, KeyboardG},
		{SDLK_h, KeyboardH},
		{SDLK_i, KeyboardI},
		{SDLK_j, KeyboardJ},
		{SDLK_k, KeyboardK},
		{SDLK_l, KeyboardL},
		{SDLK_m, KeyboardM},
		{SDLK_n, KeyboardN},
		{SDLK_o, KeyboardO},
		{SDLK_p, KeyboardP},
		{SDLK_q, KeyboardQ},
		{SDLK_r, KeyboardR},
		{SDLK_s, KeyboardS},
		{SDLK_t, KeyboardT},
		{SDLK_u, KeyboardU},
		{SDLK_v, KeyboardV},
		{SDLK_w, KeyboardW},
		{SDLK_x, KeyboardX},
		{SDLK_y, KeyboardY},
		{SDLK_z, KeyboardZ},
		{SDLK_0, Keyboard0},
		{SDLK_1, Keyboard1},
		{SDLK_2, Keyboard2},
		{SDLK_3, Keyboard3},
		{SDLK_4, Keyboard4},
		{SDLK_5, Keyboard5},
		{SDLK_6, Keyboard6},
		{SDLK_7, Keyboard7},
		{SDLK_8, Keyboard8},
		{SDLK_9, Keyboard9},
		{SDLK_KP_0, KeyboardKeypad0},
		{SDLK_KP_1, KeyboardKeypad1},
		{SDLK_KP_2, KeyboardKeypad2},
		{SDLK_KP_3, KeyboardKeypad3},
		{SDLK_KP_4, KeyboardKeypad4},
		{SDLK_KP_5, KeyboardKeypad5},
		{SDLK_KP_6, KeyboardKeypad6},
		{SDLK_KP_7, KeyboardKeypad7},
		{SDLK_KP_8, KeyboardKeypad8},
		{SDLK_KP_9, KeyboardKeypad9},
		{SDLK_F1, KeyboardF1},
		{SDLK_F2, KeyboardF2},
		{SDLK_F3, KeyboardF3},
		{SDLK_F4, KeyboardF4},
		{SDLK_F5, KeyboardF5},
		{SDLK_F6, KeyboardF6},
		{SDLK_F7, KeyboardF7},
		{SDLK_F8, KeyboardF8},
		{SDLK_F9, KeyboardF9},
		{SDLK_F10, KeyboardF10},
		{SDLK_F11, KeyboardF11},
		{SDLK_F12, KeyboardF12},
		{SDLK_F13, KeyboardF13},
		{SDLK_F14, KeyboardF14},
		{SDLK_F15, KeyboardF15},
		{SDLK_UP, KeyboardUp},
		{SDLK_DOWN, KeyboardDown},
		{SDLK_LEFT, KeyboardLeft},
		{SDLK_RIGHT, KeyboardRight},
		{SDLK_PAGEUP, KeyboardPageUp},
		{SDLK_PAGEDOWN, KeyboardPageDown},
		{SDLK_HOME, KeyboardHome},
		{SDLK_END, KeyboardEnd},
		{SDLK_RETURN, KeyboardEnter},
		{SDLK_BACKSPACE, KeyboardBackspace},
		{SDLK_INSERT, KeyboardInsert},
		{SDLK_DELETE, KeyboardDelete},
		{SDLK_TAB, KeyboardTab},
		{SDLK_SPACE, KeyboardSpace},
		{SDLK_ESCAPE, KeyboardEscape},
		{SDLK_KP_PLUS, KeyboardKeypadAdd},
		{SDLK_KP_MINUS, KeyboardKeypadSubtract},
		{SDLK_KP_MULTIPLY, KeyboardKeypadMultiply},
		{SDLK_KP_DIVIDE, KeyboardKeypadDivide},
		{SDLK_KP_PERIOD, KeyboardKeypadDecimal},
		{SDLK_KP_ENTER, KeyboardKeypadEnter},
		{SDLK_BACKQUOTE, KeyboardGrave},
		{SDLK_QUOTE, KeyboardApostrophe},
		{SDLK_SEMICOLON, KeyboardSemicolon},
		{SDLK_COMMA, KeyboardComma},
		{SDLK_PERIOD, KeyboardPeriod},
		{SDLK_SLASH, KeyboardSlash},
		{SDLK_LEFTBRACKET, KeyboardLeftBracket},
		{SDLK_RIGHTBRACKET, KeyboardRightBracket},
		{SDLK_BACKSLASH, KeyboardBackslash},
		{SDLK_MINUS, KeyboardHyphen},
		{SDLK_EQUALS, KeyboardEquals},
		{SDLK_CAPSLOCK, KeyboardCapslock},
		{SDLK_LSHIFT, KeyboardLeftShift},
		{SDLK_RSHIFT, KeyboardRightShift},
		{SDLK_LCTRL, KeyboardLeftControl},
		{SDLK_RCTRL, KeyboardRightControl},
		{SDLK_LALT, KeyboardLeftAlt},
		{SDLK_RALT, KeyboardRightAlt},
		{SDLK_PAUSE, KeyboardPause}
	};
}