#include <SDL_image.h>
#include "rhc/error.h"
#include "rhc/log.h"
#include "u/image.h"

static SDL_Surface *load_buffer(void *data, int cols, int rows) {
    // Set up the pixel format color masks for RGB(A) byte arrays.
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    int depth = 32;
    int pitch = 4 * cols;

    SDL_Surface *surf = SDL_CreateRGBSurfaceFrom(data, cols, rows, depth, pitch, rmask, gmask, bmask, amask);
    return surf;
}



uImage *u_image_new_empty_a(int cols, int rows, int layers, Allocator_s a) {
    size_t data_size = cols * rows * layers * sizeof(uColor_s);
    
    if(data_size <= 0) {
        rhc_error = "image new failed";
        log_error("image new failed, wrong data size: %zu", data_size);
        return u_image_new_invalid();
    }
    
    uImage *self = a.malloc(a, sizeof(uImage) + data_size);
    self->cols = cols;
    self->rows = rows;
    self->layers = layers;
    self->allocator = a;
    return self;
}

uImage *u_image_new_zeros_a(int cols, int rows, int layers, Allocator_s a) {
    size_t data_size = cols * rows * layers * sizeof(uColor_s);
    
    if(data_size <= 0) {
        rhc_error = "image new failed";
        log_error("image new failed, wrong data size: %zu", data_size);
        return u_image_new_invalid();
    }
    
    uImage *self = a.malloc(a, sizeof(uImage) + data_size);
    memset(&self->data[0], 0, data_size);
    self->cols = cols;
    self->rows = rows;
    self->layers = layers;
    self->allocator = a;
    return self;
}

uImage *u_image_new_clone_a(const uImage *from, Allocator_s a) {
    uImage *self = u_image_new_empty_a(from->cols, from->rows, from->layers, a);
    memcpy(self->data, from->data, u_image_data_size(from));
    return self;
}

uImage *u_image_new_file_a(int layers, const char *file, Allocator_s a) {
    assume(layers > 0, "A single layer needed");
    uImage *image = NULL;
    SDL_Surface *img = IMG_Load(file);
    if (!img) {
        rhc_error = "load image file failed";
        log_warn("load image file (%s) failed: %s", file, IMG_GetError());
        goto CLEAN_UP;
    }
    SDL_PixelFormat *f = img->format;
    if (f->BitsPerPixel != 32 || f->Amask == 0) {
        rhc_error = "load image file failed";
        log_warn("load image file (%s) failed: 8bpp and alpha needed", file);
        goto CLEAN_UP;
    }

    if (img->h % layers != 0) {
        rhc_error = "load image file failed";
        log_warn("load image file (%s) failed: rows %% layers != 0", file);
        goto CLEAN_UP;
    }

    image = u_image_new_empty_a(img->w, img->h / layers, layers, a);
    memcpy(image->data, img->pixels, sizeof(uColor_s) * img->w * img->h);

    CLEAN_UP:
    SDL_FreeSurface(img);
    return image;
}

void u_image_delete(uImage *self) {
    if(u_image_valid(self))
        self->allocator.free(self->allocator, self);
}

bool u_image_save_file(const uImage *self, const char *file) {
    if(!u_image_valid(self)) {
        rhc_error = "image save file failed";
        SDL_Log("image save file (%s) failed: invalid", file);
        return false;
    }
    SDL_Surface *img = load_buffer((void *) self->data, self->cols, self->rows * self->layers);
    int ret = IMG_SavePNG(img, file);
    SDL_FreeSurface(img);
    if (ret) {
        rhc_error = "image save file failed";
        SDL_Log("image save file (%s) failed: %s", file, IMG_GetError());
        return false;
    }
    return true;
}

bool u_image_copy(uImage *self, const uImage *from) {
    if(!u_image_valid(self) || !u_image_valid(from)
            || self->cols != from->cols
            || self->rows != from->rows
            || self->layers != from->layers
            ) {
        rhc_error = "image copy failed";
        log_error("image copy failed, invalid or different size");
        return false;
    }
    
    size_t size = u_image_data_size(self);
    memcpy(self->data, from->data, size);
    return true;
}

bool u_image_equals(const uImage *self, const uImage *from) {
    if (!u_image_valid(self) || !u_image_valid(from)
            || u_image_data_size(self) != u_image_data_size(from))
        return false;

    return memcmp(self, from, u_image_full_size(self)) == 0;
}


void u_image_rotate(uImage *self, bool right) {
    if(!u_image_valid(self))
        return;
        
    uImage *tmp = u_image_new_clone_a(self, self->allocator);
    self->cols = tmp->rows;
    self->rows = tmp->cols;
    for (int l = 0; l < self->layers; l++) {
        for (int r = 0; r < self->rows; r++) {
            for (int c = 0; c < self->rows; c++) {
                int mc = right ? r : tmp->cols - 1 - r;
                int mr = right ? tmp->rows - 1 - c : c;
                *u_image_pixel(self, l, c, r) = *u_image_pixel(tmp, l, mc, mr);
            }
        }
    }

    u_image_delete(tmp);
}

void u_image_mirror(uImage *self, bool vertical) {
    if(!u_image_valid(self))
        return;
    
    uImage *tmp = u_image_new_clone(self);

    for (int l = 0; l < self->layers; l++) {
        for (int r = 0; r < self->rows; r++) {
            for (int c = 0; c < self->rows; c++) {
                int mc = vertical ? self->cols - 1 - c : c;
                int mr = vertical ? r : self->rows - 1 - r;
                *u_image_pixel(self, l, c, r) = *u_image_pixel(tmp, l, mc, mr);
            }
        }
    }

    u_image_delete(tmp);
}
