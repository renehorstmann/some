#include "r/rect.h"
#include "u/pose.h"
#include "e/window.h"
#include "mathc/float.h"
#include "mathc/utils/camera.h"
#include "camera.h"


//
// sets up an orthogonal camera for pixel perfect rendering.
// coord system has a minimum square of [-90 : +90].
// render objects can use camera.gl for the transformation matrix.
//

struct CameraGlobals_s camera;

//
// private
//

static struct {
    float real_pixel_per_pixel;
    float left, right, bottom, top;
} L;



//
// public
//

void camera_init() {
    camera.gl = &camera.matrices.vp.m00;
    camera.matrices.v = mat4_eye();
    camera.matrices.v_inv = mat4_eye();
    camera.matrices.p = mat4_eye();
    camera.matrices.p_inv = mat4_eye();
    camera.matrices.vp = mat4_eye();
    camera.matrices.v_p_inv = mat4_eye();

    camera_update();
}

void camera_update() {
    int wnd_width = e_window.size.x;
    int wnd_height = e_window.size.y;

    float smaller_size = wnd_width < wnd_height ? wnd_width : wnd_height;
    L.real_pixel_per_pixel = floorf(smaller_size / CAMERA_SIZE);

    float width_2 = wnd_width / (2 * L.real_pixel_per_pixel);
    float height_2 = wnd_height / (2 * L.real_pixel_per_pixel);

    // begin: (top, left) with a full pixel
    // end: (bottom, right) with a maybe splitted pixel
    L.left = -floorf(width_2);
    L.top = floorf(height_2);
    L.right = width_2 + (width_2 - floorf(width_2));
    L.bottom = -height_2 - (height_2 - floorf(height_2));

    camera.matrices.p = mat4_camera_ortho(L.left, L.right, L.bottom, L.top, -1, 1);

    camera.matrices.v_inv = mat4_inv(camera.matrices.v);
    camera.matrices.p_inv = mat4_inv(camera.matrices.p);

    camera.matrices.vp = mat4_mul_mat(camera.matrices.p, camera.matrices.v_inv);

    camera.matrices.v_p_inv = mat4_mul_mat(camera.matrices.v, camera.matrices.p_inv);
}

float camera_real_pixel_per_pixel() {
    return L.real_pixel_per_pixel;
}

float camera_left() {
    return L.left;
}

float camera_right() {
    return L.right;
}

float camera_bottom() {
    return L.bottom;
}

float camera_top() {
    return L.top;
}

void camera_set_pos(float x, float y) {
    u_pose_set_xy(&camera.matrices.v, x, y);
}

void camera_set_size(float size) {
    u_pose_set_size(&camera.matrices.v, size, size);
}

void camera_set_angle(float alpha) {
    u_pose_set_angle(&camera.matrices.v, alpha);
}
