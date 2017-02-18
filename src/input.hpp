#include "input_event.hpp"

namespace ve
{
	class Input
	{
	public:
		void populateFromSDLEvents(std::vector<InputEvent> & events, SDL_Event const & sdlEvent);

		void enableTextInput(bool enable);

	private:
		static std::map<int, int> sdlKeyToKeyMap;
	};
}