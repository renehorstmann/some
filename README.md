# some

**Sdl2 OpenGL Mini Engine.**

A small C game engine, that can be compiled to Desktop, Android, Web, ... 

Using SDL2, OpenGL3.3, OpenGLES3.0, WebGL2.0 to be platform independent.

some's main goal is to create apps and games that run directly on the different platforms. 

**Also as a web version on smartphones!** 

## Getting started

Fork this repo and have fun. Have a look into some header files for some further explanation.

## Live demo 
Just the hello world [livedemo](https://renehorstmann.github.io/some) compiled with emscripten.
See [main.c](src/main.c)

## Tutorial

In the repository [some_examples](https://github.com/renehorstmann/some_examples) 
are some examples and modules to use for the some framework, like buttons, textinput, colorpicker, etc.
The examples are well documented and easy to follow.

## Sponsoring
You want to support me?
> Buy Pixelc Premium on [Google Play](https://play.google.com/store/apps/details?id=de.horsimann.pixelcpremium).
Its just like [Pixelc](https://github.com/renehorstmann/Pixelc), but with a dark background and gray scaled buttons.

> Buy coins in EXTREME COIN STACKER on  [Google Play](https://play.google.com/store/apps/details?id=de.horsimann.coinstacker).

## Example Apps

- [Pixelc](https://github.com/renehorstmann/Pixelc): A pixel art editor.
- [Swarm](https://github.com/renehorstmann/GMTKgamejam21): GMTK GameJam21.
- [Santas Day Off](https://github.com/renehorstmann/SantasDayOff): A christmas mini game.
- [JumpHare](https://github.com/renehorstmann/JumpHare): Platformer game.
- [Tea](https://github.com/renehorstmann/tea): A simple stupid tea timer.



## Install and run
Options:
- Desktop
  - [Ubuntu](#install-for-ubuntu-or-wsl-ubuntu)
  - [Windows 11 WSL Ubuntu](#install-for-windows-11-wsl-windows-subsystem-linux-ubuntu)
  - [Windows Msys2](#install-windows-msys2)
  - [Windows MSVC](#install-windows-msvc)
- [In an Android App](#install-compile-and-run-directly-on-android)
- [To an Android App](#android-app-with-androidstudio)
- [Web](#compiling-for-web)

### Install for Ubuntu or WSL Ubuntu
- Install the needed libraries in the shell:
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

- [now run the hello world demo](#run-hello-world)


### Install Windows 11 WSL (Windows Subsystem Linux) Ubuntu
With WSL you can use all the awesome unix tools in Windows with an Ubuntu terminal.
But you can not distribute your compiled programs, the users have to have WSL installed.
See Windows MSYS2 or MSVC below, if you want to create an .exe to distribute for Windows

- run Powershell as admin
```
wsl --install
```
- see [Ubuntu](#install-for-ubuntu-or-wsl-ubuntu) with the Windows Ubuntu shell


### Install Windows MSYS2
MSYS2 is a package manager for Windows, that can install the compiler Mingw-w64, 
which is a port of the GCC-Compiler with wrapper POSIX headers for Windows.

- install [MSYS2](https://www.msys2.org/)
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

- [now run the hello world demo](#run-hello-world)


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

### Install Windows MSVC
MSVC is the Microsoft C(++) Compiler, that comes with Visual Studio.
The compiler can also be installed without the IDE.

- install MSVC [Buildtools](https://visualstudio.microsoft.com/de/downloads/)
  - scroll down to "Tools for Visual Studio"
  - or install Visual Studio IDE directly if you want to use it
  - you need at least MSVC Version 2019 16.8
- install a [git client](https://git-scm.com/download/win)
- install [cmake](https://cmake.org/download/) (add to system PATH)
- install [vcpkg](https://vcpkg.io/en/getting-started.html)
    - we already installed the MSVC compiler, so no need to install Visual Studio, if you dont want that IDE

    - To install vcpk in PowerShell:
  ```sh
  cd C:\
  mkdir dev
  cd dev
  git clone https://github.com/Microsoft/vcpkg.git
  .\vcpkg\bootstrap-vcpkg.bat -disableMetrics
  cd vcpkg
  ```
- Install packages in Powershell
```sh
cd C:\dev\vcpkg
# install sdl stuff
# 	net, ttf are optional, see OPTION_SOCKET and OPTION_TTF
#	mixer is not part of the some engine, but useful
.\vcpkg install glew:x64-windows sdl2:x64-windows sdl2-image:x64-windows sdl2-net:x64-windows sdl2-ttf:x64-windows sdl2-mixer:x64-windows
# install curl (optional) for OPTION_FETCH 
.\vcpkg install curl:x64-windows
```

- [now run the hello world demo with MSVC](#run-hello-world-with-msvc)

### Run Hello World with MSVC
In the Powershell Terminal:
```sh
# clone the repo
git clone https://github.com/renehorstmann/some
# mv and make a build dir
cd some 
mkdir build
cd build
#
# run cmake
# Note: you may have to edit the toolchain path for vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake # [-DCMAKE_BUILD_TYPE=Release] for release mode
# compile
cmake --build .
# run
.\Debug\some.exe
```
> An IDE may also need the cmake option for the toolchain file

### Install, compile and run directly on Android
Useful to test and code on the go. 

In fact, most of some's code, my games and apps were developed that way!

I recommend using a good Touchscreen-Keyboard-App with the right settings.
In my case its the Samsung default keyboard (Samsung Galaxy Note 8, btw.).
My recommended settings for the Samsung Keyboard:
- Smart typing / Keyboard swipe controls: **Cursor controls**
	- so its easy to navigate to the code with swipes instead of tipping on the line and position you want to editor
- Keyboard layout and feedback / Keyboard layout: **Number keys** checked; **Alternative characters** checked
	- so you can insert glyphs with a long press on the key, instead of switching to the alt. chars. site

I used the full/paid version of [CxxDroid](https://play.google.com/store/apps/details?id=ru.iiec.cxxdroid) (from Google Play), other c compilers with SDL support may work, too

- Install F-Droid: https://f-droid.org/
- In F-Droid, install Termux (https://termux.com/)
- In Termux, clone the project
```sh
# update the system
pkg upgrade
# install basic stuff
pkg install git
# get access to the local file system of your android device from termux (needs the storage permission)
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

### Android App with AndroidStudio
Have a look at the [some_android](https://github.com/renehorstmann/some_android) AndroidStudio project generator template.

### Compiling for Web
some compiles fine as website with wasm and webgl, that can also be used on mobile browsers.

Using the [Emscripten compiler](https://emscripten.org/docs/getting_started/downloads.html)

Tested under Ubuntu and WSL Ubuntu (may work on Windows too, if the compile calls are changed).


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
-s EXPORTED_RUNTIME_METHODS=FS,ccall \
-s SDL2_IMAGE_FORMATS='["png"]' \
--preload-file ./res \
-s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 \
-lidbfs.js \
-DPLATFORM_EMSCRIPTEN -DOPTION_GLES -DOPTION_SDL \
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
-s EXPORTED_RUNTIME_METHODS=FS,ccall \
-s SDL2_IMAGE_FORMATS='["png"]' \
--preload-file ./res \
-s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=1 -s FETCH=1 \
-lidbfs.js \
-DPLATFORM_EMSCRIPTEN -DOPTION_GLES -DOPTION_SDL -DOPTION_TTF -DOPTION_FETCH \
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

### m

glsl like math library for C, see [Mathc](https://github.com/renehorstmann/Mathc)
Renamed to namespace `m`

### s

A C standard library addition, see [rhc](https://github.com/renehorstmann/rhc).
Renamed to the namespace `s`



## Author

René Horstmann

## License

This project is licensed under the MIT License - see the someLICENSE file for details

- Used third party libraries:
    - [SDL2](https://www.libsdl.org/) (zlib License)
    - [Emscripten](emscripten.org) (MIT License)
    - [nuklear](https://github.com/Immediate-Mode-UI/Nuklear) for debug gui windows (MIT License)
    - [curl](https://curl.se/docs/copyright.html) (MIT like License)
    - [sfd](https://github.com/rxi/sfd) for simple file dialogs (MIT License)
