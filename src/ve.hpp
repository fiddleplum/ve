#pragma once

#include "app.hpp"
#include "util/ptr.hpp"

// The user implements this function. It should just create a new pointer of a subclass of an App.
ve::OwnPtr<ve::App> createApp(std::vector<std::string> const & args);

namespace ve
{
	// Returns the created app. It can be used by others ONLY after the createApp function has been called.
	ve::Ptr<ve::App> getApp();
}

