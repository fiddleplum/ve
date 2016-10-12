#pragma once

#include "app.h"
#include <string>
#include <vector>

namespace ve
{
	// Called just after the application opens. Implemented by the user.
	void startup(UsePtr<App> app, std::vector<std::string> const & params);

	// Called just before the application quits. Implemented by the user.
	void shutdown(UsePtr<App> app);
}

