#pragma once

#include "app.h"
#include "util/ptr.h"
#include <string>
#include <vector>

namespace ve
{
	// Called just after the application opens. Implemented by the user.
	void startup(std::vector<std::string> const & params);

	// Called just before the application quits. Implemented by the user.
	void shutdown();

	// Called once a frame. Implemented by the user.
	void update(float dt);
}