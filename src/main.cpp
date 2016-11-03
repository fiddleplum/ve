#include "ve.h"
#include "app_internal.h"
#include "ptr.h"
#include <SDL.h>
#include <vector>
#include <string>

ve::OwnPtr<ve::AppInternal> app;

ve::UsePtr<ve::App> ve::getApp()
{
	return app;
}

ve::UsePtr<ve::AppInternal> ve::getAppInternal()
{
	return app;
}

// Called by SDL to run the entire app.
int main(int argc, char *argv[])
{
#undef main
	std::vector<std::string> args;
	try
	{
		// Grab the params. Don't include the 0th arg, because it is the program name.
		for (int i = 1; i < argc; ++i)
		{
			args.push_back(std::string(argv[i]));
		}

		// Initialize the singletons.
		//InputSystem::createInstance();
		//shaderCache.setNew();
		//textureCache.setNew();
		//fontCache.setNew();
		//SceneModelCache::createInstance();

		//SDL_InitSubSystem(SDL_INIT_AUDIO);

		app = ve::OwnPtr<ve::AppInternal>::createNew();

		ve::startup(args);

		app->loop();

		ve::shutdown();

		app.setNull();

		//scenes.clear();
		//ve::windows.clear();
		// Destroy the singletons.
		//fontCache.setNull();
		//textureCache.setNull();
		//shaderCache.setNull();
		//SceneModelCache::destroyInstance();
		//InputSystem::destroyInstance();

		// Stop SDL.
		SDL_Quit();
	}
	catch (std::exception const & e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", e.what(), nullptr);
		ve::shutdown();
		app.setNull();
		return -1;
	}
	return 0;
}