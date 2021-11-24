#ifndef E_CORE_H
#define E_CORE_H

//
// this file just includes SDL and OpenGL headers
//

#define GL_GLEXT_PROTOTYPES

#ifdef OPTION_GLEW

#include <GL/glew.h>

#endif

#include <SDL2/SDL.h>

#ifdef OPTION_TTF
#include <SDL2/SDL_ttf.h>
#endif

#ifdef OPTION_SOCKET
#include <SDL2/SDL_net.h>
#endif

#ifndef OPTION_GLEW
#include <SDL2/SDL_opengl.h>
#endif


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
// #include <emscripten/html5.h>
#endif


static void e_exit_failure() {
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
    EM_ASM(
            set_error_img();
            );
#endif
    exit(EXIT_FAILURE);
}


#endif //E_CORE_H
