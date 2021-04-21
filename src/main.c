#include "e/e.h"
#include "r/r.h"
#include "u/u.h"
#include "rhc/rhc.h"

#include "camera.h"


// example code:
// 'R'ender'o'bject 
// renders text via RoBatch
static RoText text;
// stores the last pressed mouse click / touch to render with RoText text
static ePointer_s last_click;

static RoSingle ro;


// will be called on mouse or touch events
static void on_pointer_callback(ePointer_s pointer, void *ud) {
    if (pointer.action != E_POINTER_DOWN)
        return;
    last_click = pointer;
    printf("clicked at x=%f, y=%f, id=%i, is touch: %i\n", pointer.pos.x, pointer.pos.y, pointer.id, e_input.is_touch);
}
//

static void main_loop(float delta_time);


int main(int argc, char **argv) {
    log_info("some");

    // init e (environment)
    e_window_init("some");
    e_input_init();
    e_gui_init();

    // init r (render)
    r_render_init(e_window.window);

    // init systems
    camera_init();

/*
    // example code
    // class init of RoText
    // RoText *self, int max_chars, const float *camera_vp_matrix
    ro_text_init_font55(&text, 128, camera.gl);
    // see u/pose.h, sets a mat4 transformation pose
    u_pose_set_xy(&text.pose, camera_left() + 20, 0);*/

    // setup a pointer listener
  e_input_register_pointer_event(on_pointer_callback, NULL);

    // set clear color
    r_render.clear_color = (vec4) {0.5, 0.75, 0.5, 1};
    //
    
    ro = ro_single_new(camera.gl, r_texture_new_invalid());
    ro.rect.pose = u_pose_new(64, 0, 32, 32);

    e_window_main_loop(main_loop);

    e_gui_kill();

    return 0;
}


static void main_loop(float delta_time) {
    // e updates
    e_input_update();

    // simulate
    camera_update();


    // render
    r_render_begin_frame(e_window.size.x, e_window.size.y);

/*
    // example code
    static float val = 10;
    //creates a debug window to set val
    // min, max, step
    e_gui_wnd_float_attribute("val", &val, 0, 100, 5);
    char buf[128];
    sprintf(buf, "Hello World\nval=%5.1f\nspace pressed: %i\nid=%i x=%.2f y=%.2f",
            val, e_input.keys.space, last_click.id, last_click.pos.x, last_click.pos.y);
    // RoText methods: set text, render
    ro_text_set_text(&text, buf);
    ro_text_render(&text);
    //
*/

    ivec2 tex_size = r_render.framebuffer_tex.size;
    void *buffer = rhc_malloc_raising(4 * tex_size.x*tex_size.y);
    r_texture2d_get(r_render.framebuffer_tex, buffer);
    ro_single_set_texture(&ro, r_texture_new(tex_size.x, tex_size.y, 1, 1, buffer));
    rhc_free(buffer);
    
    ro_single_render(&ro);
    r_render.clear_color.rgb = vec3_random_range(0, 1);

    // uncomment to clone the current framebuffer into r_render.framebuffer_tex
    r_render_blit_framebuffer(e_window.size.x, e_window.size.y);

    e_gui_render();

    // swap buffers
    r_render_end_frame();

    // check for opengl errors:
    r_render_error_check("main_loop_end");
}


