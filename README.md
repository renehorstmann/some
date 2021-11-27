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
```

```sh
emcc -I../include/ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_NET=2 -s FULL_ES3=1 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file ../res -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 -DOPTION_GLES -DOPTION_SDL -DOPTION_TTF -DOPTION_SOCKET ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -o index.html
```

Add the following changes to the generated index.html:
```html
<style>
  #canvas {
    position: absolute;
    top: 0px;
    left: 0px;
    margin: 0px;
    width: 100%;
    height: 100%;
    overflow: hidden;
    display: block;
  }
</style>
<script>
    function set_exit_failure_error_msg() {
        var newContent = '<!DOCTYPE html><html><body style="background-color:black;"><h1 style="color:white;">Potato Browsers are not supported!</h1><p style="color:silver;">Full WebGL2.0 is needed!</p></body></html>';
        document.open();
        document.write(newContent);
        document.close();
    }
</script>
```
This will let Emscripten run in fullscreen and display an error text, if the app / game is not able to run (WebGL2.0 support missing)

## Without Cmake

Instead of cmake, the following call to gcc may work, too.

```sh
mkdir build && cd build

cp -r ../res .

gcc ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -I../include/ $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lSDL2_net -lglew32 -lopengl32 -lglu32 -DOPTION_GLEW -DOPTION_SDL -DOPTION_TTF -DOPTION_SOCKET -o some
```

## Author

René Horstmann

## Licence

This project is licensed under the MIT License - see the someLICENSE file for details

- Used third party libraries:
    - [SDL2](https://www.libsdl.org/) (zlib License)
    - [Emscripten](emscripten.org) (MIT License)
    - [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) for debug gui windows (MIT Licence)
