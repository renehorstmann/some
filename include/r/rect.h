#ifndef R_RECT_H
#define R_RECT_H

//
// rect pod structs, used by some renderobjects (ro)
//

#include <stddef.h> // offsetof
#include "mathc/types/float.h"

// basic rect for rendering
typedef struct {
    mat4 pose;      // 3d pose for the rect position (see u/pose.h)
    mat4 uv;        // 3d pose for the uv texture map (see u/pose.h)
    vec4 color;     // additional color (texture_color * color)
} rRect_s;

// rect with additional values for a particle system
typedef struct {
    union {
        rRect_s rect;
        struct {
            mat4 pose;
            mat4 uv;
            vec4 color;
        };
    };
    vec4 speed;         // position += speed * dt
    vec4 acc;           // position += acc * dt * dt
    vec4 axis_angle;    // orientation += axis_angle * dt   (nx, ny, nz, rad)
    vec4 color_speed;   // color += color_speed * dt
    vec2 uv_step;       // uv_position += uv_step * floor(dt / uv_time)
    float uv_time;      // using uv_* may result in artifacts, if dt is too high (float accuracy)
    float start_time;   // dt = current_time - start_time
} rParticleRect_s;

// checks padding
_Static_assert(offsetof(rParticleRect_s, uv_time)
               - offsetof(rParticleRect_s, uv_step)
               == 2 * sizeof(float),
               "uv_time must not be padded");

// creates a new rect with:
// pose = uv = eye
// color = 1
rRect_s r_rect_new();

// as new, but xy=MAX, alpha=0
rRect_s r_rect_new_hidden();

// creates a new particle rect with:
// pose = uv = eye
// color = 1
// speed, acc, color_speed, uv_step = 0
// axis_angle = 0, 0, 1, 0
// uv_time = max
// start_time = 0
rParticleRect_s r_particlerect_new();

// as new, but xy=MAX, alpha=0
rParticleRect_s r_particlerect_new_hidden();


#endif //R_RECT_H
