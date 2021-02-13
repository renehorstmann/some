#include "e/e.h"
#include "r/r.h"
#include "u/u.h"

#include "camera.h"


static rRoText text;


static void main_loop(float delta_time);


int main(int argc, char **argv) {
    SDL_Log("some");

    // init e (environment)
    e_window_init("some");
    e_input_init();
    e_gui_init();

    // init r (render)
    r_render_init(e_window.window);

    // init systems
    camera_init();
    
    // example code
    r_ro_text_init_font55(&text, 128, camera.gl);
    u_pose_set_xy(&text.pose, camera_left() + 20, 0);
    //
    

    e_window_main_loop(main_loop);

    e_gui_kill();
    
    return 0;
}


static void main_loop(float delta_time) {
    // e updates
    e_window_update();
    e_input_update();
    

    // simulate
    camera_update();
    

    // render
    r_render_begin_frame(e_window.size.x, e_window.size.y);


    // example code (e_gui creates a debug window to set val)
    static float val = 10;
    e_gui_wnd_float_attribute("val", &val, 0, 100, 5);
    char buf[128];
    sprintf(buf, "Hello World\nval=%.1f", val);
    r_ro_text_set_text(&text, buf);
    r_ro_text_render(&text);
    //
    


    e_gui_render();

    // swap buffers
    r_render_end_frame();

    // check for opengl errors:
    r_render_error_check();
}


