#ifndef SOME_CAMERA_H
#define SOME_CAMERA_H

//
// PixelPerfect canvas camera with view matrix.
// to control the camera position and size
//

#include <stdbool.h>
#include "mathc/types/float.h"
#include "mathc/types/int.h"


#define CAMERA_SIZE 180 // *4=720; *6=1080; *8=1440

typedef struct {
    mat4 v;         // view / pose of the camera in 3d space
    mat4 v_inv;     // inv(v)
    mat4 p;         // projection of the camera (perspective / orthogonal)
    mat4 p_inv;     // inv(p)
    mat4 vp;        // p @ v_inv   used for render objects (ro)
    mat4 v_p_inv;   // v @ p_inv   used for input
} CameraMatrices_s;

typedef struct {
    CameraMatrices_s matrices;

    struct {
        float scale;    // units per pixel
        float left, right, bottom, top;
    } RO;   // read only
} Camera_s;

// creates a new camera
Camera_s camera_new();

// updates the camera matrices
void camera_update(Camera_s *self, ivec2 window_size);

// set the position of the camera in the world
void camera_set_pos(Camera_s *self, float x, float y);

// set the size / zoom of the camera
void camera_set_size(Camera_s *self, float size);

// set the rotation of the camera
void camera_set_angle(Camera_s *self, float alpha);

// returns the width in game pixel (!=real pixel)
static float camera_width(const Camera_s *self) {
    return -self->RO.left + self->RO.right;
}

// returns the height in game pixel (!=real pixel)
static float camera_height(const Camera_s *self) {
    return -self->RO.bottom + self->RO.top;
}

// returns true if the camera is in portrait mode (smartphone)
static bool camera_is_portrait_mode(const Camera_s *self) {
    return camera_height(self) > camera_width(self);
}

#endif //SOME_CAMERA_H
