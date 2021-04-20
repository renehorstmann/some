#ifndef RHC_ALLOC_IMPL_H
#define RHC_ALLOC_IMPL_H

#include <stdlib.h>
#include "../error.h"
#include "../log.h"
#include "../allocator.h"

#ifdef OPTION_SDL
#include <SDL.h>
#endif

void *rhc_allocator_default_alloc_impl_(Allocator_s self, size_t size) {
#ifdef OPTION_SDL
    void *data = SDL_malloc(size);
#else
    void *data = malloc(size);
#endif
    if(!data) {
        log_error("allocation failed for a size of: %zu", size);
        rhc_error = "allocation error";
    }
    return data;
}

void *rhc_allocator_default_realloc_impl_(Allocator_s self, void *memory, size_t size) {
#ifdef OPTION_SDL
    void *data = SDL_realloc(memory, size);
#else
    void *data = realloc(memory, size);
#endif
    if(!data) {
        log_error("reallocation failed for a size of: %zu", size);
        rhc_error = "allocation error";
        return memory;
    }
    return data;
}

void rhc_allocator_default_free_impl_(Allocator_s self, void *memory) {
#ifdef OPTION_SDL
    SDL_free(memory);
#else
    free(memory);
#endif
}

void *rhc_allocator_raising_alloc_impl_(Allocator_s self, size_t size) {
#ifdef OPTION_SDL
    void *data = SDL_malloc(size);
#else
    void *data = malloc(size);
#endif
    assume(data, "allocation failed for a size of: %zu", size);
    return data;
}

void *rhc_allocator_raising_realloc_impl_(Allocator_s self, void *memory, size_t size) {
#ifdef OPTION_SDL
    void *data = SDL_realloc(memory, size);
#else
    void *data = realloc(memory, size);
#endif
    assume(data, "reallocation failed for a size of: %zu", size);
    return data;
}

#endif //RHC_ALLOC_IMPL_H
