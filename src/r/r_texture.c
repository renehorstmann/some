#include <SDL_image.h>
#include "rhc/error.h"
#include "rhc/log.h"
#include "r/render.h"
#include "r/texture.h"



rTexture r_texture_new(int image_cols, int image_rows, int sprites_cols, int sprites_rows, const void *opt_buffer) {
    r_render_error_check("r_texture_newBEGIN");
    
    assume(
    image_cols > 0 && image_rows > 0
    && sprites_cols > 0 && sprites_rows > 0
    && image_cols % sprites_cols == 0
    && image_rows % sprites_rows == 0
    && image_cols / sprites_cols >= 1
    && image_rows / sprites_rows >= 1
    , "texture size invalid");
    
    rTexture self = {
        0,
        {{image_cols/sprites_cols, image_rows/sprites_rows}},
        {{sprites_cols, sprites_rows}}
    };
    
    
    // todo: redraw buffer (may be NULL!)
    assume(self.sprites.x == 1, "not implemented yet");
    
    glGenTextures(1, &self.tex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, self.tex);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA,
             self.sprite_size.x, 
             self.sprite_size.y, 
             self.sprites.x * self.sprites.y,
             0, GL_RGBA, GL_UNSIGNED_BYTE, opt_buffer);

    // GL_REPEAT is already default...
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    r_texture_filter_nearest(self);
    r_render_error_check("r_texture_new");
    return self;
}

rTexture r_texture_new_sdl_surface(int sprites_cols, int sprites_rows, const SDL_Surface *img) {
    assume(img, "SDL_Surface must not be null");
    SDL_PixelFormat *f = img->format;
    if (f->BitsPerPixel != 32 || f->Amask == 0) {
        rhc_error = "load texture failed";
        log_error("Load texture failed, 8bpp and alpha needed");
        return r_texture_new_invalid();
    }
    return r_texture_new(img->w, img->h, sprites_cols, sprites_rows, img->pixels);
}

rTexture r_texture_new_file(int sprites_cols, int sprites_rows, const char *file) {
    SDL_Surface *img = IMG_Load(file);
    if (!img) {
        rhc_error = "load texture failed";
        log_error("Load image (%s) failed: %s", file, IMG_GetError());
        return r_texture_new_invalid();
    }

    rTexture self = r_texture_new_sdl_surface(sprites_cols, sprites_rows, img);
    
    SDL_FreeSurface(img);
    return self;
}

rTexture r_texture_new_empty(int image_cols, int image_rows, int sprites_cols, int sprites_rows) {
    return r_texture_new(image_cols, image_rows, sprites_cols, sprites_rows, NULL);
}

rTexture r_texture_new_white_pixel() {
    return r_texture_new(1, 1, 1, 1, (uint8_t[]) {255, 255, 255, 255});
}

void r_texture_kill(rTexture *self) {
    // invalid safe
    glDeleteTextures(1, &self->tex);
    *self = r_texture_new_invalid();
}

void r_texture_update(rTexture self, const void *buffer) {
    r_render_error_check("r_texture_updateBEGIN");
    if(!r_texture_valid(self))
        return;
    glBindTexture(GL_TEXTURE_2D_ARRAY, self.tex);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 
            0, 0, 0, 
            self.sprite_size.x, 
            self.sprite_size.y, 
            self.sprites.x * self.sprites.y,
            GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    r_render_error_check("r_texture_update");
}

void r_texture_filter_linear(rTexture self) {
    r_render_error_check("r_texture_filter_linearBEGIN");
    if(!r_texture_valid(self))
        return;
    glBindTexture(GL_TEXTURE_2D_ARRAY, self.tex);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    r_render_error_check("r_texture_filter_linear");
}

void r_texture_filter_nearest(rTexture self) {
    r_render_error_check("r_texture_filter_nearestBEGIN");   
    if(!r_texture_valid(self))
        return;
    glBindTexture(GL_TEXTURE_2D_ARRAY, self.tex);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    r_render_error_check("r_texture_filter_nearest");
}

