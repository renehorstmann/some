# some

Sdl2 OpenGL Mini Engine.

A small C game engine, that can be compiled to Desktop, Android, Web, ... Using SDL2, OpenGL3.3, OpenGLES3.0, WebGL2.0
to be platform independent.

## Getting started

Fork this repo and have fun. Have a look into some header files for some further explanation.


## Tutorial

In the repository [some_examples](https://github.com/renehorstmann/some_examples) are some examples and modules to use for the some framework, like buttons, etc.
The examples are well documented and easy to follow.

## Example Apps

- [Pixelc](https://github.com/renehorstmann/Pixelc): A pixel art editor.
- [Tilec](https://github.com/renehorstmann/Tilec): A tile map editor.
- [Swarm](https://github.com/renehorstmann/GMTKgamejam21): GMTK GameJam21.
- [Santas Day Off](https://github.com/renehorstmann/SantasDayOff): A christmas mini game.
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
    - e_simple
    - r_render
- Pseudo random value: [static _Thread_local uint32_t x](src/u/u_prandom.c)
- [rhc](https://github.com/renehorstmann/rhc) globals

## Compiling for Web

Using Emscripten:

```sh
mkdir web && cp index.html web && cp icon/* web && cd web
```

```sh
cp -r ../res .
```

```sh
emcc -I../include/ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s FULL_ES3=1 -s EXPORTED_FUNCTIONS='["_main", "_e_io_idbfs_synced", "_e_io_file_upload_done"]' -s EXPORTED_RUNTIME_METHODS=FS -s SDL2_IMAGE_FORMATS='["png"]'  --preload-file ./res -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 -s FETCH=1 -lidbfs.js -DOPTION_GLES -DOPTION_SDL -DOPTION_TTF -DOPTION_FETCH ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -o index.js
```

test the website:
```sh
python3 -m http.server --bind localhost  # [port]
```

## Without Cmake

Instead of cmake, the following call to gcc may work, too.

```sh
mkdir build && cd build

cp -r ../res .

gcc ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -I../include/ $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lcurl -lglew32 -lopengl32 -lglu32 -DOPTION_GLEW -DOPTION_SDL -DOPTION_TTF -DOPTION_FETCH -o some
```

## Author

René Horstmann

## Licence

This project is licensed under the MIT License - see the someLICENSE file for details

- Used third party libraries:
    - [SDL2](https://www.libsdl.org/) (zlib License)
    - [Emscripten](emscripten.org) (MIT License)
    - [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) for debug gui windows (MIT Licence)
