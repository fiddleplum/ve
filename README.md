# VE, the Visualization Engine

## Architecture

VE is written in C++. The dependencies are:
* SDL for the platforming, windowing, and OpenGL

All classes are in a namespace `ve`. The user must implement the function `ve::main` which is called internally by the library. From there, the user can create a window and start the application loop.

