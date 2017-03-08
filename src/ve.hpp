#pragma once

#include "app.hpp"
#include "log.hpp"
#include "util/ptr.hpp"
#include <vector>
#include <string>

// User implemented. Called just before the game loop.
void entry(ve::Ptr<ve::App> const & app, ve::Config const & config);

// User implemented. Called just after the game loop.
void exit();

