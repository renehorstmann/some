#include "e/simple.h"

// hello world
#include "r/ro_text.h"
#include "u/pose.h"

#include "camera.h"

static struct {
    RoText text;
} L;


// this function will be called at the start of the app
static void init() {
    s_log("init");

    // init systems
    camera_init();

    L.text = ro_text_new_font55(128);
    ro_text_set_text(&L.text, "Hello World");
}


// this function is called either each frame or at a specific update/s time
static void update(float dtime) {
    // simulate
    camera_update();

    u_pose_set_xy(&L.text.pose, camera.RO.left + 5, camera.RO.top - 5);
}


// this function is called each frame to render stuff, dtime is the time between frames
static void render(float dtime) {
    mat4 *camera_mat = &camera.matrices.vp;

    ro_text_render(&L.text, camera_mat);
}


int main(int argc, char **argv) {
    e_simple_start("some", "Horsimann",
                   1.0f,   // startup block time (the time in which "Horsimann" is displayed at startup)
                   0,      // update deltatime_ms, <=0 to turn off and use fps (5=200hz)
                   init, update, render);
    return 0;
}
