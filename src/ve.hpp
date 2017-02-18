#pragma once

#include "app.hpp"
#include "util/ptr.hpp"
#include <vector>
#include <string>

// User implemented. Called just before the game loop.
void entry(ve::Ptr<ve::App> const & app, std::vector<std::string> const & args);

// User implemented. Called just after the game loop.
void exit();

