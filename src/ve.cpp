#include "ve.hpp"
#include "util/config.hpp"
#include <SDL.h>
#include <filesystem>

ve::OwnPtr<ve::App> app;

// Called by SDL to run the entire app.
int main(int argc, char *argv[])
{
	try
	{
		ve::Config config;
		std::string configFilename = "config.txt";
		if (std::experimental::filesystem::exists(configFilename))
		{
			config.load(configFilename);
		}

		auto app = ve::OwnPtr<ve::App>::returnNew(config);

		entry(app, config);

		app->loop();

		app.setNull();

		return 0;
	}
	catch (std::exception const & e)
	{
		//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error! Uncaught exception: ", e.what(), nullptr);
		//return -1;
		throw;
	}
}

