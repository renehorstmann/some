#ifndef RHC_ALLOC_IMPL_H
#define RHC_ALLOC_IMPL_H

#include <stdlib.h>
#include "../error.h"
#include "../log.h"
#include "../allocator.h"

void *rhc_allocator_default_alloc_impl_(Allocator_s self, size_t size) {
    void *data = malloc(size);
    if(!data) {
        log_error("allocation failed for a size of: %zu", size);
        rhc_error = "allocation error";
    }
    return data;
}

void *rhc_allocator_default_realloc_impl_(Allocator_s self, void *memory, size_t size) {
    void *data = realloc(memory, size);
    if(!data) {
        log_error("reallocation failed for a size of: %zu", size);
        rhc_error = "allocation error";
        return memory;
    }
    return data;
}

void rhc_allocator_default_free_impl_(Allocator_s self, void *memory) {
    free(memory);
}

void *rhc_allocator_raising_alloc_impl_(Allocator_s self, size_t size) {
    void *data = malloc(size);
    assume(data, "allocation failed for a size of: %zu", size);
    return data;
}

void *rhc_allocator_raising_realloc_impl_(Allocator_s self, void *memory, size_t size) {
    void *data = realloc(memory, size);
    assume(data, "reallocation failed for a size of: %zu", size);
    return data;
}

#endif //RHC_ALLOC_IMPL_H
