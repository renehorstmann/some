#ifndef SOME_CAMERA_H
#define SOME_CAMERA_H

//
// PixelPerfect canvas camera with view matrix.
// to control the camera position and size
//

#include <stdbool.h>
#include "mathc/types/float.h"


#define CAMERA_SIZE 180 // *4=720; *6=1080; *8=1440

typedef struct {
    mat4 v;         // view / pose of the camera in 3d space
    mat4 v_inv;     // inv(v)
    mat4 p;         // projection of the camera (perspective / orthogonal)
    mat4 p_inv;     // inv(p)
    mat4 vp;        // p @ v_inv   used for render objects (ro)
    mat4 v_p_inv;   // v @ p_inv   used for input
} CameraMatrices_s;

struct CameraGlobals_s {
    CameraMatrices_s matrices;
    const float *gl;
};
extern struct CameraGlobals_s camera;


void camera_init();

void camera_update();

float camera_real_pixel_per_pixel();

float camera_left();

float camera_right();

float camera_bottom();

float camera_top();

static float camera_width() {
    return -camera_left() + camera_right();
}

static float camera_height() {
    return -camera_bottom() + camera_top();
}

static bool camera_is_portrait_mode() {
    return camera_height() > camera_width();
}

void camera_set_pos(float x, float y);

void camera_set_size(float size);

void camera_set_angle(float alpha);

#endif //SOME_CAMERA_H
