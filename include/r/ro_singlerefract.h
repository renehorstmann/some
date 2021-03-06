#ifndef R_RO_SINGLEREFRACT_H
#define R_RO_SINGLEREFRACT_H

//
// class to render a single rect with a draw call
// its like ro_single, but with additional reflection / refraction support.
//
//// uses 3 textures in total
//// 1: default texture
//// 2: refraction map
////    r: 128+offset for x refraction
////    g: 128+offset for y
////    b: x_stretch_value + y_stretch_value * 16
////          normal: 12+12*16    (8+4)
////          mirror x: 4+12*16   (8-4)
////    a: refraction alpha
//// 3: framebuffer texture, to grab pixels for the refraction
////    defaults to r_render.framebuffer_tex
//// view_aabb is the screen space in which the rect is rendered
////    in texture space (origin is top left) [0:1]
////    as center_x, _y, radius_x, _y
////    defaults to fullscreen (0.5, 0.5, 0.5, 0.5)
//

#include "rhc/allocator.h"
#include "ro_types.h"


RoSingleRefract ro_singlerefract_new(
        const float *scale_ptr,
        rTexture tex_main_sink, rTexture tex_refraction_sink);

void ro_singlerefract_kill(RoSingleRefract *self);

void ro_singlerefract_render(RoSingleRefract *self, const mat4 *camera_mat);

// resets the texture, if .owns_tex_main is true, it will delete the old texture
void ro_singlerefract_set_texture_main(RoSingleRefract *self, rTexture tex_main_sink);

// resets the texture, if .owns_tex_refraction is true, it will delete the old texture
void ro_singlerefract_set_texture_refraction(RoSingleRefract *self, rTexture tex_refraction_sink);


#endif //R_RO_SINGLEREFRACT_H
