# some

Sdl2 OpenGl Mini Engine.

A small C game engine, that can be compiled to Desktop, Android, Web, ... Using SDL2, OpenGL3.3, OpenGLES3.0, WebGL2.0
to be platform independent.

## Getting started

Fork this repo and have fun. Have a look into some header files for some further explanation.

## Example Apps

- [Pixelc](https://github.com/renehorstmann/Pixelc): A pixel art editor.
- [Tilec](https://github.com/renehorstmann/Tilec): A tile map editor.
- [Swarm](https://github.com/renehorstmann/GMTKgamejam21): GMTK GameJam21.
- [JumpHare](https://github.com/renehorstmann/JumpHare): Platformer game.

## Naming

### Functions

A function like: `e_window_new` got its name of:

- `e`: as namespace for the package e
- `window`: module name
- `new`: module function/method name

### Components

Each component may have some basic function names:

```c
*_new       // returns a new component
*_new_*     // special new
*_kill      // deinialize a comp.

*_update    // update the internals
*_render    // ...
```

### More Style rules

For more style rules used in my projects, see [rhc/style](https://github.com/renehorstmann/rhc)

## Packages

The some framework consists of a few packages, each for specific tasks:

### e

e_* is for window and environment management, like window, input, sdl stuff...

- e_gui: [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) based debug gui

### r

r_* is for OpenGL rendering ro_* is for render objects

### u

Utilities like setting up a 2D pose or loading an image

### p

Physics (may be expanded later)

### mathc

glsl like math library for C, see [Mathc](https://github.com/renehorstmann/Mathc)

### rhc

A C standard library addition, see [rhc](https://github.com/renehorstmann/rhc)

## Globals

The following globals are used:

- singletons for some modules:
    - e_window
    - e_input
    - e_gui
    - r_render
- Pseudo random value: [static _Thread_local uint32_t x](src/u/u_prandom.c)
- [rhc](https://github.com/renehorstmann/rhc) globals

## Compiling for Web

Using Emscripten:

```sh
mkdir web && cd web

emcc -I../include/ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s FULL_ES3=1 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file ../res -s ALLOW_MEMORY_GROWTH=1 -DOPTION_GLES -DOPTION_SDL -DOPTION_TTF ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -o index.html
```

May / will not work on Apple, because of their poor WebGL2 support.

## Compiling on Windows

Compiling with Mingw (msys2). Currently not working with cmake, but with the following gcc call.

```sh
mkdir build && cd build

cp -r ../res .

gcc ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -I../include/ $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lglew32 -lopengl32 -lglu32 -DOPTION_GLEW -DOPTION_SDL -DOPTION_TTF -o some
```

## Author

René Horstmann

## Licence

This project is licensed under the MIT License - see the someLICENSE file for details

- Used third party libraries:
    - [SDL2](https://www.libsdl.org/) (zlib License)
    - [Emscripten](emscripten.org) (MIT License)
    - [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) for debug gui windows (MIT Licence)
