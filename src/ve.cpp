#include "ve.hpp"
#include <SDL.h>

ve::OwnPtr<ve::App> app;

// Called by SDL to run the entire app.
int main(int argc, char *argv[])
{
	try
	{
		// Grab the params. Don't include the 0th arg, because it is the program name.
		std::vector<std::string> args;
		for (int i = 1; i < argc; ++i)
		{
			args.push_back(std::string(argv[i]));
		}

		auto app = ve::OwnPtr<ve::App>::returnNew();

		entry(app, args);

		app->loop();

		app.setNull();

		return 0;
	}
	catch (std::exception const & e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error! Uncaught exception: ", e.what(), nullptr);
		return -1;
	}
}

