#include "event.h"
#include <SDL.h>
#include <map>

namespace ve
{
	extern std::map<int, std::string> keysToStrings;
	extern std::map<int, int> sdlKeysToKeys;

	InputEvent::InputEvent(InputEvent::Type type)
		: type(type)
	{
	}

	bool InputEvent::is(InputEvent::Type type) const
	{
		return this->type == type;
	}

	KeyboardEvent::KeyboardEvent()
		: InputEvent(Keyboard)
	{
	}

	void KeyboardEvent::setFromSDL(int type, int sym)
	{
		pressed = (type == SDL_KEYDOWN);
		auto iter = sdlKeysToKeys.find(sym);
		if (iter != sdlKeysToKeys.end())
		{
			key = iter->second;
		}
		else
		{
			key = Unknown;
		}
	}

	std::string KeyboardEvent::toString() const
	{
		return "Keyboard " + keysToStrings[key] + ": " + (pressed ? "Pressed" : "Unpressed");
	}

	TextEvent::TextEvent()
		: InputEvent(Text)
	{
	}

	std::string TextEvent::toString() const
	{
		return "Text: " + text;
	}

	MouseButtonEvent::MouseButtonEvent()
		: InputEvent(MouseButton)
	{
	}

	void MouseButtonEvent::setFromSDL(int type, int button_)
	{
		pressed = (type == SDL_MOUSEBUTTONDOWN);
		switch (button_)
		{
			case SDL_BUTTON_LEFT: button = Left; break;
			case SDL_BUTTON_MIDDLE: button = Middle; break;
			case SDL_BUTTON_RIGHT: button = Right; break;
			case SDL_BUTTON_X1: button = Button0 + 3; break;
			case SDL_BUTTON_X2: button = Button0 + 4; break;
		}
	}

	std::string MouseButtonEvent::toString() const
	{
		std::string r = "Mouse Button ";
		switch(button)
		{
			case Left: r += "Left"; break;
			case Middle: r += "Middle"; break;
			case Right: r += "Right"; break;
			default: r += std::to_string(button); break;
		}
		r += std::string(": ") + (pressed ? "Pressed" : "Unpressed");
		return r;
	}

	MouseMoveEvent::MouseMoveEvent()
		: InputEvent(MouseMove)
	{
	}

	void MouseMoveEvent::setFromSDL(int xrel, int yrel)
	{
		offset[0] = xrel;
		offset[1] = yrel;
	}

	std::string MouseMoveEvent::toString() const
	{
		return "Mouse Move: " + std::to_string(offset[0]) + ", " + std::to_string(offset[1]);
	}

	MouseWheelEvent::MouseWheelEvent()
		: InputEvent(MouseWheel)
	{
	}

	void MouseWheelEvent::setFromSDL(int amount)
	{
		up = (amount > 0);
	}

	std::string MouseWheelEvent::toString() const
	{
		return std::string("Mouse Wheel: ") + (up ? "Up" : "Down");
	}

	ControllerButtonEvent::ControllerButtonEvent()
		: InputEvent(ControllerButton)
	{
	}

	std::string ControllerButtonEvent::toString() const
	{
		return "Controller " + std::to_string(controller) + " Button " + std::to_string(button) + ": " + (pressed ? "Pressed" : "Unpressed");
	}

	ControllerAxisEvent::ControllerAxisEvent()
		: InputEvent(ControllerAxis)
	{
	}

	std::string ControllerAxisEvent::toString() const
	{
		return "Controller " + std::to_string(controller) + " Axis " + std::to_string(axis) + ": " + std::to_string(value);
	}

	std::map<int, std::string> keysToStrings = {
		{KeyboardEvent::Unknown, "Unknown"},
		{KeyboardEvent::A, "A"},
		{KeyboardEvent::B, "B"},
		{KeyboardEvent::C, "C"},
		{KeyboardEvent::D, "D"},
		{KeyboardEvent::E, "E"},
		{KeyboardEvent::F, "F"},
		{KeyboardEvent::G, "G"},
		{KeyboardEvent::H, "H"},
		{KeyboardEvent::I, "I"},
		{KeyboardEvent::J, "J"},
		{KeyboardEvent::K, "K"},
		{KeyboardEvent::L, "L"},
		{KeyboardEvent::M, "M"},
		{KeyboardEvent::N, "N"},
		{KeyboardEvent::O, "O"},
		{KeyboardEvent::P, "P"},
		{KeyboardEvent::Q, "Q"},
		{KeyboardEvent::R, "R"},
		{KeyboardEvent::S, "S"},
		{KeyboardEvent::T, "T"},
		{KeyboardEvent::U, "U"},
		{KeyboardEvent::V, "V"},
		{KeyboardEvent::W, "W"},
		{KeyboardEvent::X, "X"},
		{KeyboardEvent::Y, "Y"},
		{KeyboardEvent::Z, "Z"},
		{KeyboardEvent::N0, "0"},
		{KeyboardEvent::N1, "1"},
		{KeyboardEvent::N2, "2"},
		{KeyboardEvent::N3, "3"},
		{KeyboardEvent::N4, "4"},
		{KeyboardEvent::N5, "5"},
		{KeyboardEvent::N6, "6"},
		{KeyboardEvent::N7, "7"},
		{KeyboardEvent::N8, "8"},
		{KeyboardEvent::N9, "9"},
		{KeyboardEvent::Up, "Up"},
		{KeyboardEvent::Down, "Down"},
		{KeyboardEvent::Left, "Left"},
		{KeyboardEvent::Right, "Right"},
		{KeyboardEvent::PageUp, "Page Up"},
		{KeyboardEvent::PageDown, "Page Down"},
		{KeyboardEvent::Home, "Home"},
		{KeyboardEvent::End, "End"},
		{KeyboardEvent::Enter, "Enter"},
		{KeyboardEvent::Backspace, "Backspace"},
		{KeyboardEvent::Insert, "Insert"},
		{KeyboardEvent::Delete, "Delete"},
		{KeyboardEvent::Tab, "Tab"},
		{KeyboardEvent::Space, "Space"},
		{KeyboardEvent::Escape, "Escape"},
		{KeyboardEvent::Keypad0, "Keypad 0"},
		{KeyboardEvent::Keypad1, "Keypad 1"},
		{KeyboardEvent::Keypad2, "Keypad 2"},
		{KeyboardEvent::Keypad3, "Keypad 3"},
		{KeyboardEvent::Keypad4, "Keypad 4"},
		{KeyboardEvent::Keypad5, "Keypad 5"},
		{KeyboardEvent::Keypad6, "Keypad 6"},
		{KeyboardEvent::Keypad7, "Keypad 7"},
		{KeyboardEvent::Keypad8, "Keypad 8"},
		{KeyboardEvent::Keypad9, "Keypad 9"},
		{KeyboardEvent::KeypadAdd, "Keypad Plus"},
		{KeyboardEvent::KeypadSubtract, "Keypad Minus"},
		{KeyboardEvent::KeypadMultiply, "Keypad Multiply"},
		{KeyboardEvent::KeypadDivide, "Keypad Divide"},
		{KeyboardEvent::KeypadDecimal, "Keypad Decimal"},
		{KeyboardEvent::KeypadEnter, "Keypad Enter"},
		{KeyboardEvent::Grave, "Grave"},
		{KeyboardEvent::Apostrophe, "Apostrophe"},
		{KeyboardEvent::Semicolon, "Semicolon"},
		{KeyboardEvent::Comma, "Comma"},
		{KeyboardEvent::Period, "Period"},
		{KeyboardEvent::Slash, "Slash"},
		{KeyboardEvent::LeftBracket, "Left Bracket"},
		{KeyboardEvent::RightBracket, "Right Bracket"},
		{KeyboardEvent::Backslash, "Backslash"},
		{KeyboardEvent::Hyphen, "Hyphen"},
		{KeyboardEvent::Equals, "Equals"},
		{KeyboardEvent::Capslock, "Capslock"},
		{KeyboardEvent::LeftShift, "Left Shift"},
		{KeyboardEvent::RightShift, "Right Shift"},
		{KeyboardEvent::LeftControl, "Left Control"},
		{KeyboardEvent::RightControl, "Right Control"},
		{KeyboardEvent::LeftAlt, "Left Alt"},
		{KeyboardEvent::RightAlt, "Right Alt"},
		{KeyboardEvent::F1, "F1"},
		{KeyboardEvent::F2, "F2"},
		{KeyboardEvent::F3, "F3"},
		{KeyboardEvent::F4, "F4"},
		{KeyboardEvent::F5, "F5"},
		{KeyboardEvent::F6, "F6"},
		{KeyboardEvent::F7, "F7"},
		{KeyboardEvent::F8, "F8"},
		{KeyboardEvent::F9, "F9"},
		{KeyboardEvent::F10, "F10"},
		{KeyboardEvent::F11, "F11"},
		{KeyboardEvent::F12, "F12"},
		{KeyboardEvent::F13, "F13"},
		{KeyboardEvent::F14, "F14"},
		{KeyboardEvent::F15, "F15"},
		{KeyboardEvent::Pause, "Pause"}
	};

	std::map<int, int> sdlKeysToKeys = {
		{SDLK_a, KeyboardEvent::A},
		{SDLK_b, KeyboardEvent::B},
		{SDLK_c, KeyboardEvent::C},
		{SDLK_d, KeyboardEvent::D},
		{SDLK_e, KeyboardEvent::E},
		{SDLK_f, KeyboardEvent::F},
		{SDLK_g, KeyboardEvent::G},
		{SDLK_h, KeyboardEvent::H},
		{SDLK_i, KeyboardEvent::I},
		{SDLK_j, KeyboardEvent::J},
		{SDLK_k, KeyboardEvent::K},
		{SDLK_l, KeyboardEvent::L},
		{SDLK_m, KeyboardEvent::M},
		{SDLK_n, KeyboardEvent::N},
		{SDLK_o, KeyboardEvent::O},
		{SDLK_p, KeyboardEvent::P},
		{SDLK_q, KeyboardEvent::Q},
		{SDLK_r, KeyboardEvent::R},
		{SDLK_s, KeyboardEvent::S},
		{SDLK_t, KeyboardEvent::T},
		{SDLK_u, KeyboardEvent::U},
		{SDLK_v, KeyboardEvent::V},
		{SDLK_w, KeyboardEvent::W},
		{SDLK_x, KeyboardEvent::X},
		{SDLK_y, KeyboardEvent::Y},
		{SDLK_z, KeyboardEvent::Z},
		{SDLK_0, KeyboardEvent::N0},
		{SDLK_1, KeyboardEvent::N1},
		{SDLK_2, KeyboardEvent::N2},
		{SDLK_3, KeyboardEvent::N3},
		{SDLK_4, KeyboardEvent::N4},
		{SDLK_5, KeyboardEvent::N5},
		{SDLK_6, KeyboardEvent::N6},
		{SDLK_7, KeyboardEvent::N7},
		{SDLK_8, KeyboardEvent::N8},
		{SDLK_9, KeyboardEvent::N9},
		{SDLK_KP_0, KeyboardEvent::Keypad0},
		{SDLK_KP_1, KeyboardEvent::Keypad1},
		{SDLK_KP_2, KeyboardEvent::Keypad2},
		{SDLK_KP_3, KeyboardEvent::Keypad3},
		{SDLK_KP_4, KeyboardEvent::Keypad4},
		{SDLK_KP_5, KeyboardEvent::Keypad5},
		{SDLK_KP_6, KeyboardEvent::Keypad6},
		{SDLK_KP_7, KeyboardEvent::Keypad7},
		{SDLK_KP_8, KeyboardEvent::Keypad8},
		{SDLK_KP_9, KeyboardEvent::Keypad9},
		{SDLK_F1, KeyboardEvent::F1},
		{SDLK_F2, KeyboardEvent::F2},
		{SDLK_F3, KeyboardEvent::F3},
		{SDLK_F4, KeyboardEvent::F4},
		{SDLK_F5, KeyboardEvent::F5},
		{SDLK_F6, KeyboardEvent::F6},
		{SDLK_F7, KeyboardEvent::F7},
		{SDLK_F8, KeyboardEvent::F8},
		{SDLK_F9, KeyboardEvent::F9},
		{SDLK_F10, KeyboardEvent::F10},
		{SDLK_F11, KeyboardEvent::F11},
		{SDLK_F12, KeyboardEvent::F12},
		{SDLK_F13, KeyboardEvent::F13},
		{SDLK_F14, KeyboardEvent::F14},
		{SDLK_F15, KeyboardEvent::F15},
		{SDLK_UP, KeyboardEvent::Up},
		{SDLK_DOWN, KeyboardEvent::Down},
		{SDLK_LEFT, KeyboardEvent::Left},
		{SDLK_RIGHT, KeyboardEvent::Right},
		{SDLK_PAGEUP, KeyboardEvent::PageUp},
		{SDLK_PAGEDOWN, KeyboardEvent::PageDown},
		{SDLK_HOME, KeyboardEvent::Home},
		{SDLK_END, KeyboardEvent::End},
		{SDLK_RETURN, KeyboardEvent::Enter},
		{SDLK_BACKSPACE, KeyboardEvent::Backspace},
		{SDLK_INSERT, KeyboardEvent::Insert},
		{SDLK_DELETE, KeyboardEvent::Delete},
		{SDLK_TAB, KeyboardEvent::Tab},
		{SDLK_SPACE, KeyboardEvent::Space},
		{SDLK_ESCAPE, KeyboardEvent::Escape},
		{SDLK_KP_PLUS, KeyboardEvent::KeypadAdd},
		{SDLK_KP_MINUS, KeyboardEvent::KeypadSubtract},
		{SDLK_KP_MULTIPLY, KeyboardEvent::KeypadMultiply},
		{SDLK_KP_DIVIDE, KeyboardEvent::KeypadDivide},
		{SDLK_KP_PERIOD, KeyboardEvent::KeypadDecimal},
		{SDLK_KP_ENTER, KeyboardEvent::KeypadEnter},
		{SDLK_BACKQUOTE, KeyboardEvent::Grave},
		{SDLK_QUOTE, KeyboardEvent::Apostrophe},
		{SDLK_SEMICOLON, KeyboardEvent::Semicolon},
		{SDLK_COMMA, KeyboardEvent::Comma},
		{SDLK_PERIOD, KeyboardEvent::Period},
		{SDLK_SLASH, KeyboardEvent::Slash},
		{SDLK_LEFTBRACKET, KeyboardEvent::LeftBracket},
		{SDLK_RIGHTBRACKET, KeyboardEvent::RightBracket},
		{SDLK_BACKSLASH, KeyboardEvent::Backslash},
		{SDLK_MINUS, KeyboardEvent::Hyphen},
		{SDLK_EQUALS, KeyboardEvent::Equals},
		{SDLK_CAPSLOCK, KeyboardEvent::Capslock},
		{SDLK_LSHIFT, KeyboardEvent::LeftShift},
		{SDLK_RSHIFT, KeyboardEvent::RightShift},
		{SDLK_LCTRL, KeyboardEvent::LeftControl},
		{SDLK_RCTRL, KeyboardEvent::RightControl},
		{SDLK_LALT, KeyboardEvent::LeftAlt},
		{SDLK_RALT, KeyboardEvent::RightAlt},
		{SDLK_PAUSE, KeyboardEvent::Pause},
	};
}

