#pragma once

#include "util/vector.hpp"
#include <string>
#include <map>
#include <vector>

union SDL_Event;

/*

The flat class has the benefit of being put into a vector and stored. You can't do that with the hierarchy, except with OwnPtrs.
The flat class cannot deal with controller adding/removing, as it doesn't fit in the paradigm.
It is required to have a key-mapping system, and that requires storing input events, which is only possible with a flat class.



Solution: Use the flat class, but make the ints private and settable only in constructor. Use accessors that make sense, like isPressed(), etc.


*/





namespace ve
{
	class InputEvent
	{
	public:
		// Constructs a new event from params.
		InputEvent(int device, int axis, int value);

		// Constructs a new event from params, specifically for KeyboardText events.
		InputEvent(int device, int axis, std::string const & text);

		// Returns the device as an enumeration of Device.
		int getDevice() const;

		// Returns the axis as an enumeration of KeyboardAxis, MouseAxis, or ControllerAxis, depending on the device.
		int getAxis() const;

		// Returns the value of the given device and axis.
		int getValue() const;

		// Returns the text entered for a KeyboardText event.
		std::string getText() const;

		// Returns true if value is 1, false otherwise. Used for keyboard keys, mouse buttons, and controller buttons.
		bool isPressed() const;

		// Returns true if this and the other have the same device and axis. Can be used for input event mapping.
		bool isSameDeviceAndAxis(InputEvent const & event) const;

		// Returns a human readable version of the input event.
		std::string toString() const;

	private:
		static std::map<int, std::string> keyToStringMap;

		int device;
		int axis;
		int value;
		std::string text;
	};

	enum Device
	{
		DeviceKeyboard,
		DeviceMouse,
		DeviceController0 // Use DeviceController0 + i to get controller i (starting with 0).
	};

	enum KeyboardAxis
	{
		KeyboardA, KeyboardB, KeyboardC, KeyboardD, KeyboardE, KeyboardF, KeyboardG, KeyboardH, KeyboardI, KeyboardJ, KeyboardK, KeyboardL, KeyboardM, KeyboardN, KeyboardO, KeyboardP, KeyboardQ, KeyboardR, KeyboardS, KeyboardT, KeyboardU, KeyboardV, KeyboardW, KeyboardX, KeyboardY, KeyboardZ,
		Keyboard0, Keyboard1, Keyboard2, Keyboard3, Keyboard4, Keyboard5, Keyboard6, Keyboard7, Keyboard8, Keyboard9, // Regular number keys
		KeyboardUp, KeyboardDown, KeyboardLeft, KeyboardRight, KeyboardPageUp, KeyboardPageDown,
		KeyboardHome, KeyboardEnd, KeyboardEnter, KeyboardBackspace, KeyboardInsert, KeyboardDelete, KeyboardTab, KeyboardSpace, KeyboardEscape,
		KeyboardKeypad0, KeyboardKeypad1, KeyboardKeypad2, KeyboardKeypad3, KeyboardKeypad4, KeyboardKeypad5, KeyboardKeypad6, KeyboardKeypad7, KeyboardKeypad8, KeyboardKeypad9, // Keypad number keys
		KeyboardKeypadAdd, KeyboardKeypadSubtract, KeyboardKeypadMultiply, KeyboardKeypadDivide, KeyboardKeypadDecimal, KeyboardKeypadEnter,
		KeyboardGrave, KeyboardApostrophe, KeyboardSemicolon, KeyboardComma, KeyboardPeriod, KeyboardSlash,
		KeyboardLeftBracket, KeyboardRightBracket, KeyboardBackslash, KeyboardHyphen, KeyboardEquals,
		KeyboardCapslock, KeyboardLeftShift, KeyboardRightShift, KeyboardLeftControl, KeyboardRightControl,
		KeyboardLeftAlt /*!	same as option */, KeyboardRightAlt /*! same as alt gr */,
		KeyboardF1, KeyboardF2, KeyboardF3, KeyboardF4, KeyboardF5, KeyboardF6, KeyboardF7, KeyboardF8, KeyboardF9, KeyboardF10, KeyboardF11, KeyboardF12, KeyboardF13, KeyboardF14, KeyboardF15,
		KeyboardPause,
		KeyboardText
	};

	enum MouseAxis
	{
		MouseX, MouseY, MouseWheel, MouseLeft, MouseMiddle, MouseRight, MouseButton0 = MouseLeft // Use MouseButton0 + i to get mouse button i (starting with 0).
	};

	enum ControllerAxis
	{
		ControllerButton0, // Use ControllerButton0 + i to get controller button i (starting with 0).
		ControllerAxis0 = 256 // Use ControllerAxis0 + i to get controller axis i (starting with 0).
	};
}

/*

	Notes on pass-thru

	MouseMove should make its cursor invalid if the widget that it is handling it deems so.
	Keyboard should stop after the first widget consumes it.
	MouseButton

	Should every widget have a contains function? Then in a widget container, if a widget contains the cursor, the MouseMove is automatically
	changed to cursorValid = false, and MouseButton left click sets that widget as the focus widget. Then all keyboard commands are sent to the focus widget
	first, then to the other widgets (if it isn't consumed already)

*/