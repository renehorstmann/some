#ifndef SOME_CAMERA_H
#define SOME_CAMERA_H

//
// PixelPerfect camera with view matrix.
// to control the camera position and size
//

#include "s/s.h"
#include "m/types/float.h"
#include "m/types/int.h"

// the camera will unsure a canvas of at least CAMERA_SIZE * CAMERA_SIZE units
#define CAMERA_SIZE 180 // *4=720; *6=1080; *8=1440


typedef struct {
    // view / pose of the camera in 3d space
    mat4 v;
    mat4 v_inv;     // inv(v)

    // projection of the camera (perspective / orthogonal)
    // pass 'p' to a render object, 
    //     to draw it at a static position on the screen (HUD)
    // use 'p_inv' to transform a touch position to the screen units
    mat4 p;
    mat4 p_inv;     // inv(p)

    // combination of view and projection
    // pass 'vp' to a render object,
    //     to draw it in the world, 
    //     so that the camera movement of 'v' is used
    // use 'v_p_inv' to transform a touch position to the world
    mat4 vp;        // p @ v_inv
    mat4 v_p_inv;   // v @ p_inv
} CameraMatrices_s;

struct Camera_Globals {
    CameraMatrices_s matrices;

    struct {
        // units per pixel
        float scale;

        // static camera borders in units
        // bottom and right may be a little behind the actual screen border (depending on the real resolution)
        int left, right, bottom, top;

        // static camera borders in units
        // bottom and right are on the actual screen borders
        float screen_left, screen_right, screen_bottom, screen_top;
    } RO;   // read only
};
extern struct Camera_Globals camera;


// initializes camera
void camera_init();

// updates the camera matrices
void camera_update();

// set the position of the camera in the world
void camera_set_pos(float x, float y);

// set the size / zoom of the camera (default = 1.0)
void camera_set_size(float size);

// set the rotation of the camera
void camera_set_angle(float alpha);

// returns the width in game pixel (!=real pixel)
static int camera_width() {
    return -camera.RO.left + camera.RO.right;
}

// returns the height in game pixel (!=real pixel)
static int camera_height() {
    return -camera.RO.bottom + camera.RO.top;
}

// returns true if the camera is in portrait mode (smartphone)
static bool camera_is_portrait_mode() {
    return camera_height() > camera_width();
}

#endif //SOME_CAMERA_H
