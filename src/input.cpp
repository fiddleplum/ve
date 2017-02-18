#include "input.hpp"
#include <SDL.h>

namespace ve
{
	void Input::populateFromSDLEvents(std::vector<InputEvent> & events, SDL_Event const & sdlEvent)
	{
		int axis;
		int value;
		std::string text;
		auto iter = sdlKeyToKeyMap.begin();
		switch (sdlEvent.type)
		{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				if (sdlEvent.key.repeat == 0)
				{
					iter = sdlKeyToKeyMap.find(sdlEvent.key.keysym.sym);
					if (iter != sdlKeyToKeyMap.end())
					{
						axis = iter->second;
						value = (sdlEvent.type == SDL_KEYDOWN ? 1 : 0);
						events.push_back(InputEvent(DeviceKeyboard, axis, value));
					}
				}
				break;
			case SDL_TEXTINPUT:
				axis = KeyboardText;
				text = sdlEvent.text.text;
				events.push_back(InputEvent(DeviceKeyboard, axis, text));
				break;
			case SDL_MOUSEMOTION:
				axis = MouseX;
				value = sdlEvent.motion.xrel;
				events.push_back(InputEvent(DeviceMouse, axis, value));
				axis = MouseY;
				value = sdlEvent.motion.yrel;
				events.push_back(InputEvent(DeviceMouse, axis, value));
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
				events.push_back(InputEvent(DeviceMouse, axis, value));
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