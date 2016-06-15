# VE, the Visualization Engine

## Architecture

VE is written in C++. The dependencies are:
* SDL for the platforming, windowing, and OpenGL

All classes are in a namespace `ve`. The user must implement the function `ve::main` which is called internally by the library. From there, the user can create windows and scenes and start the application loop.

The game loop happens like so:

1. For each input event (keyboard, mouse, controller)
  1. Each window calls handleEvent
  1. Each scene calls handleEvent
1. Each window calls update
1. Each scene calls update
1. Each window calls preRender
1. Each scene calls preRender
1. Each window calls render (which renders any scenes via viewports)

Each window contains a root GUI element, which may in turn contain other elements.

