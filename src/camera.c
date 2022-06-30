#include "e/window.h"
#include "e/gui.h"
#include "u/pose.h"
#include "m/utils/camera.h"
#include "m/sca/float.h"
#include "camera.h"


// comment out to not use a pixel perfect camera
#define PIXEL_PERFECT

//
// public
//

struct Camera_Globals camera;

void camera_init() {
    camera.matrices.v = mat4_eye();
    camera.matrices.v_inv = mat4_eye();
    camera.matrices.p = mat4_eye();
    camera.matrices.p_inv = mat4_eye();
    camera.matrices.vp = mat4_eye();
    camera.matrices.v_p_inv = mat4_eye();
}

void camera_update() {
    int wnd_width = e_window.size.x;
    int wnd_height = e_window.size.y;

    float smaller_size = wnd_width < wnd_height ? wnd_width : wnd_height;
    camera.RO.scale = smaller_size / CAMERA_SIZE;

#ifdef PIXEL_PERFECT
    if (camera.RO.scale > 1) {
        camera.RO.scale = sca_floor(camera.RO.scale);
    }
#endif

    // set nuklear scale for the debug gui windows
    e_gui.scale = camera.RO.scale/3;

    float width_2 = wnd_width / (2 * camera.RO.scale);
    float height_2 = wnd_height / (2 * camera.RO.scale);

    // begin: (top, left) with a full pixel
    // end: (bottom, right) with a maybe splitted pixel
    camera.RO.screen_left = -floorf(width_2);
    camera.RO.screen_top = floorf(height_2);
    camera.RO.screen_right = width_2 + (width_2 - floorf(width_2));
    camera.RO.screen_bottom = -height_2 - (height_2 - floorf(height_2));

    camera.matrices.p = mat4_camera_ortho(camera.RO.screen_left, camera.RO.screen_right, camera.RO.screen_bottom,
                                          camera.RO.screen_top, -1, 1);

    camera.matrices.v_inv = mat4_inv(camera.matrices.v);
    camera.matrices.p_inv = mat4_inv(camera.matrices.p);

    camera.matrices.vp = mat4_mul_mat(camera.matrices.p, camera.matrices.v_inv);

    camera.matrices.v_p_inv = mat4_mul_mat(camera.matrices.v, camera.matrices.p_inv);

    camera.RO.left = sca_floor(camera.RO.screen_left);
    camera.RO.right = sca_ceil(camera.RO.screen_right);
    camera.RO.bottom = sca_floor(camera.RO.screen_bottom);
    camera.RO.top = sca_ceil(camera.RO.screen_top);
}


void camera_set_pos(float x, float y) {
#ifdef PIXEL_PERFECT
    // reset x and y to match a real pixel
    x = sca_floor(x * camera.RO.scale) / camera.RO.scale;
    y = sca_floor(y * camera.RO.scale) / camera.RO.scale;
#endif
    u_pose_set_xy(&camera.matrices.v, x, y);
}

void camera_set_size(float size) {
    u_pose_set_size(&camera.matrices.v, size, size);
}

void camera_set_angle(float alpha) {
    u_pose_set_angle(&camera.matrices.v, alpha);
}
