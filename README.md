# some

Sdl2 OpenGL Mini Engine.

A small C game engine, that can be compiled to Desktop, Android, Web, ... Using SDL2, OpenGL3.3, OpenGLES3.0, WebGL2.0
to be platform independent.

## Getting started

Fork this repo and have fun. Have a look into some header files for some further explanation.

## Live demo 
Just the hello world [livedemo](https://renehorstmann.github.io/some) compiled with emscripten.
See [main.c](src/main.c)

## Tutorial

In the repository [some_examples](https://github.com/renehorstmann/some_examples) 
are some examples and modules to use for the some framework, like buttons, textinput, etc.
The examples are well documented and easy to follow.

## Example Apps

- [Pixelc](https://github.com/renehorstmann/Pixelc): A pixel art editor.
- [Swarm](https://github.com/renehorstmann/GMTKgamejam21): GMTK GameJam21.
- [Santas Day Off](https://github.com/renehorstmann/SantasDayOff): A christmas mini game.
- [JumpHare](https://github.com/renehorstmann/JumpHare): Platformer game.
- [Tea](https://github.com/renehorstmann/tea): A simple stupid tea timer.
- [Tilec](https://github.com/renehorstmann/Tilec): A tile map editor (deprecated...).


## Install for compiling on Desktop

### Install WSL (Windows Subsystem Linux) Ubuntu on Windows 11:
With WSL you can use all the awesome unix tools in Windows with a Ubuntu terminal.
But you can not distribute your compiled programs, the users have to have WSL installed.
See Windows MSYS2 below, if you want to create an .exe to distribute for Windows

- run Powershell as admin
```
wsl --install
```

### In Ubuntu or WSL Ubuntu
```sh
# update the system
sudo apt update && sudo apt upgrade
# install basic stuff for c coding
sudo apt install build-essential gdb git cmake 
# install sdl stuff 
# 	net, ttf are optional, see OPTION_SOCKET and OPTION_TTF
#	mixer is not part of the some engine, but useful
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-net-dev libsdl2-ttf-dev libsdl2-mixer-dev
# install curl (optional) for OPTION_FETCH 
sudo apt install libcurl4-openssl-dev
```

### Windows MSYS2
- install https://www.msys2.org/
- update the package manager (as the site says: `pacman -Syu` ; restart ; `pacman -Su`)
- In "MSYS2 MSYS" Terminal
```sh
# basic stuff for c coding
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
pacman -S git mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
# install sdl stuff
# 	net, ttf are optional, see OPTION_SOCKET and OPTION_TTF
#	mixer is not part of the some engine, but useful
pacman -S mingw-w64-x86_64-glew mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer
# install curl (optional) for OPTION_FETCH 
pacman -S mingw-w64-x86_64-curl
```
- add the following line at the end of the file .bashrc (runs on terminal startup) (run `nano ~/.bashrc`)
```
# add mingw32 lib to the path, in this dir should be the opengl libraries (dll)
export PATH=$PATH:/c/msys64/mingw64/x86_64-w64-mingw32/lib
```
- Close the terminal
- If you use an IDE, you may need to add the PATH to ...mingw32/lib (see above) into the windows system variables

### Run Hello World
In Ubuntu, WSL Ubuntu or "MSYS2 MinGW x64" Terminal:
```sh
# clone the repo
git clone https://github.com/renehorstmann/some
# mv and make a build dir
cd some && mkdir build && cd build
# run cmake
cmake .. # [-DCMAKE_BUILD_TYPE=Release] for release mode
# compile
cmake --build . # [-- -j16] to compile with 16 threads... 
# run
./some
```

## Install, compile and run directly on Android
- I used the full/paid version of CxxDroid (from Google Play), other c compilers with SDL support may work, too
- Install F-Droid: https://f-droid.org/
- In F-Droid, install Termux (https://termux.com/)
- In Termux, clone the project
```sh
# update the system
pkg upgrade
# install basic stuff
pkg install git
# get access to the local file system of your android device from termux
termux-setup-storage
# cd to your file system
cd ~/storage/shared
# optional, create a c projects folder
mkdir cprojects && cd cprojects
# clone the repo
git clone https://github.com/renehorstmann/some
```
- In CxxDroid, install the following libraries (Menu/Install Libraries)
  - SDL2
  - SDL2-image
  - SDL2-net (optional with OPTION_SOCKET)
  - SDL2-ttf (optional with OPTION_TTF)
  - SDL2-mixer (not part of the some engine, but useful)
  - libcurl (optional with OPTION_FETCH)
- Open a file from some in CxxDroid
- You should now be able to compile and run directly on Android


## Compiling for Web
Using Emscripten https://emscripten.org/

Tested under Ubuntu and WSL Ubuntu.
You should have already cloned the project and `cd` to that dir:

- Create a sub directory to compile the website
```sh
mkdir web && cp index.html web && cp icon/* web && cd web
```

- Copy all resources, because emscripten may not be able to use `../res`
```sh
cp -r ../res .
```

- Compile
```sh
emcc -O3 \
-I../include/ \
-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s FULL_ES3=1 -s \
EXPORTED_FUNCTIONS='["_main", "_e_io_idbfs_synced", "_e_io_file_upload_done"]' \
-s EXPORTED_RUNTIME_METHODS=FS \
-s SDL2_IMAGE_FORMATS='["png"]' \
--preload-file ./res \
-s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 \
-lidbfs.js \
-DOPTION_GLES -DOPTION_SDL \
../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c \
-o index.js
```

- Test the website (open a browser and call localhost:8000)
```sh
python3 -m http.server --bind localhost  # [port]
```

- Compile with all options:
> fetch is more reliable than websockets, so SDL_net and OPTION_SOCKET are missing here
```sh
emcc -O3 \
-I../include/ \
-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 -s FULL_ES3=1 -s \
EXPORTED_FUNCTIONS='["_main", "_e_io_idbfs_synced", "_e_io_file_upload_done"]' \
-s EXPORTED_RUNTIME_METHODS=FS \
-s SDL2_IMAGE_FORMATS='["png"]' \
--preload-file ./res \
-s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 -s FETCH=1 \
-lidbfs.js \
-DOPTION_GLES -DOPTION_SDL -DOPTION_TTF -DOPTION_FETCH \
../src/e/*.c ../src/p/*.c ../src/r/*.c ../src/u/*.c ../src/*.c \
-o index.js
```

## Naming

### Functions

A function like: `e_window_init` got its name of:

- `e`: as namespace for the package e
- `window`: module name
- `init`: module function/method name

### Components

Each component may have some basic function names:

```c
*_init      // initializes a (single member) module
*_new       // returns a new component
*_new_*     // special new
*_kill      // deinitialize a comp.

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



## Author

René Horstmann

## License

This project is licensed under the MIT License - see the someLICENSE file for details

- Used third party libraries:
    - [SDL2](https://www.libsdl.org/) (zlib License)
    - [Emscripten](emscripten.org) (MIT License)
    - [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) for debug gui windows (MIT License)
    - [curl](https://curl.se/docs/copyright.html) (MIT like License)
