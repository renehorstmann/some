#include "e/simple.h"

// hello world
#include "r/ro_text.h"
#include "u/pose.h"

#include "camera.h"

static struct {
    Camera_s camera;
    RoText text;
} L;


// this function will be called at the start of the app
static void init(eSimple *simple, ivec2 window_size) {
    // init systems
    L.camera = camera_new();

    L.text = ro_text_new_font55(128);
    ro_text_set_text(&L.text, "Hello World");
}


// this functions is called either each frame or at a specific update/s time
static void update(eSimple *simple, ivec2 window_size, float dtime) {
    // simulate
    camera_update(&L.camera, window_size);

    u_pose_set_xy(&L.text.pose, L.camera.RO.left + 5, L.camera.RO.top-5);
}


// this function is calles each frame to render stuff, dtime is the time between frames
static void render(eSimple *simple, ivec2 window_size, float dtime) {
    mat4 *camera_mat = &L.camera.matrices.vp;

    ro_text_render(&L.text, camera_mat);

    // uncomment to clone the current framebuffer into r_render_get_framebuffer_tex
    // r_render_blit_framebuffer(simple->render, window_size);
}


int main(int argc, char **argv) {
    e_simple_start("some", "Horsimann",
            1.0f,   // startup block time (the time in which "Horsimann" is displayed at startup)
            0,      // updates/s, <=0 to turn off and use fps
            init, update, render);
    return 0;
}
