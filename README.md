# some
Sdl2 OpenGl Mini Engine.

A small C game engine, that can be compiled to Desktop, Android, Web, ...
Using SDL2, OpenGL3.3, OpenGLES3.0, WebGL2.0 to be platform independent.

## Getting started
Fork this repo and have fun.
Have a look into some header files for some further explanation.

## Example Apps
- [Pixelc](https://github.com/renehorstmann/Pixelc): A pixel art editor.
- [Tilec](https://github.com/renehorstmann/Tilec): A tile map editor.
- [JumpHare](https://github.com/renehorstmann/JumpHare): Platformer game.


## Todo
- shader -> program with single file shaders
  - currently not working in gles
- adapt to rhc

## Naming
### Functions
A function like: `e_window_init` got its name of:
- `e`: as namespace for the submodule e
- `window`: module name
- `init`: module function name

### Components
Each component may have some basic function names:
```c
*_init      // initialize the given component
*_init_*    // special init
*_new       // as *_init, but returns the new component
*_new_*     // special new
*_kill      // deinialize a comp.
*_delete    // for components, created with new (may be on heap)

*_update    // update the internals
*_render    // ...
```

### Modules
Modules may consist of globals and locals.
Globals are packed in a struct with the module name: `e_window.size`.
Locals are packed in a static struct called `L`.

### Classes
Class data names are PascalCase. (with a lowercase submodule name, if its a single letter).

Classes have the same naming as modules, but always take the class data struct as first parameter:

`vec2 ro_text_set_text(RoText *self, const char *text);`

here, `ro` is the special namespace as r(ender)o(bject) within the r submodule.
Classes should be killed or deleted at some point.

### PlainOldData
Automatic "pod" struct names are either complete lowercase or PascalCase_s.
for example: `rRect_s`.
These automatic structs must not be killed or deleted.


## e
e_* is for window and environment management, like window, input, sdl stuff...
- e_gui: [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) based debug gui
## r
r_* is for OpenGL rendering
ro_* is for render objects

## u
Utilities like setting up a 2D pose

## p
Physics (may be expanded later)

## mathc
glsl like math library for C, see [Mathc](https://github.com/renehorstmann/Mathc)

## rhc
A C standard library addition, see [rhc](https://github.com/renehorstmann/rhc)


## Compiling for Web
Using Emscripten:
```
emcc -I../include/ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s FULL_ES3=1 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file ../res -s ALLOW_MEMORY_GROWTH=1 -DOPTION_GLES -DOPTION_TTF ../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c -o index.html
```
May / will not work on Apple, because of their poor WebGL2 support.
 
## Compiling on Windows
Compiling with Mingw (msys2).
Currently not working with cmake, but with the following gcc call.
I had to put all source files into one dir (from src/e/*, r/*, p/*, u/* into src/*) to get the linker happy.
```
gcc -o some src/* -Iinclude $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lglew32 -lopengl32 -lglu32 -DOPTION_GLEW
```

## Author
René Horstmann

## Licence
This project is licensed under the MIT License - see the someLICENSE file for details

- Used third party libraries:
  - [SDL2](https://www.libsdl.org/) (zlib License)
  - [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) for debug gui windows (MIT Licence)
