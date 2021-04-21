#ifndef R_TEXTURE_H
#define R_TEXTURE_H

//
// load and setup textures
//

#include <stdint.h>
#include <stdbool.h>
#include "core.h"
#include "mathc/types/int.h"

typedef struct {
    GLuint tex;
    ivec2 sprite_size;
    ivec2 sprites;
} rTexture;

static bool r_texture_valid(rTexture self) {
    return self.tex > 0
            && self.sprite_size.x > 0 && self.sprite_size.y > 0
            && self.sprites.x > 0 && self.sprites.y > 0;
}

static rTexture r_texture_new_invalid() {
    return (rTexture) {0};
}

rTexture r_texture_new(int image_cols, int image_rows, int sprites_cols, int sprites_rows, const void *opt_buffer);

rTexture r_texture_new_sdl_surface(int sprites_cols, int sprites_rows, const SDL_Surface *img);

rTexture r_texture_new_file(int sprites_cols, int sprite_rows, const char *file);

rTexture r_texture_new_empty(int image_cols, int image_rows, int sprites_cols, int sprites_rows);

rTexture r_texture_new_white_pixel();

void r_texture_kill(rTexture *self);

void r_texture_update(rTexture self, const void *buffer);

void r_texture_filter_linear(rTexture self);

void r_texture_filter_nearest(rTexture self);

#endif //R_TEXTURE_H
