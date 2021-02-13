# some
Sdl2 OpenGl Mini Engine.


## Getting started
Fork this repo and have fun

## e
e_* is for window and environment management, like window, input, sdl stuff...

## r
r_* is dor OpenGL rendering

## u
Utilities like setting up a 2D pose

## p
Physics

## mathc
glsl like math library for C, see [Mathc](https://github.com/renehorstmann/Mathc)

## utilc
C utility functions like dynamic arrays, see [Utilc](https://github.com/renehorstmann/Utilc)


## Compiling on Windows
Compiling with Mingw (msys2).
Currently not working with cmake, but with the following gcc call.
I had to put all source files into one dir (from src/e/*, r/*, p/*, u/* into src/*) to get the linker happy.
```
gcc -o some src/* -Iinclude $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lglew32 -lopengl32 -lglu32 -DUSING_GLEW
```

## Author
René Horstmann

## Licence
This project is licensed under the MIT License - see the LICENSE file for details
