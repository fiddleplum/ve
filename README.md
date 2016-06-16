# VE, the Visualization Engine

## Architecture

VE is written in C++. The dependencies are:
* SDL for the platforming, windowing, and OpenGL

All classes are in a namespace `ve`. The user must implement the function `ve::main` which is called internally by the library. From there, the user can create windows and scenes and start the application loop.

### Game Loop

1. For each input event (keyboard, mouse, controller)
  1. Each window calls handleEvent
    1. Each gui element handles events front to back
  1. Each scene calls handleEvent
1. Each window calls update
  1. Each gui element updates (no order specified)
1. Each scene calls update
1. Each window calls postUpdate
  1. Each gui element post-updates (no order specified)
1. Each scene calls postUpdate
1. Each window calls render
  1. Each gui element is rendered back to front (any viewport renders its scenes)

Each window contains a root GUI element, which may in turn contain other elements.

### Resources

All resource classes have a corresponding static `resource` object which handles the loading/creating and destroying of that type of resource.

