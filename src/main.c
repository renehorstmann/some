#include "e/e.h"
#include "r/r.h"
#include "u/u.h"
#include "rhc/rhc.h"
#include "mathc/mathc.h"

#include "camera.h"


static struct {
    eWindow *window;
    eInput *input;
    eGui *gui;
    rRender *render;

    Camera_s camera;

    // example code:
    // 'R'ender'o'bject
    // renders text via RoBatch
    RoText text;
    RoText text85;
    // stores the last pressed mouse click / touch to render with RoText text
    ePointer_s last_click;
} L;


// will be called on mouse or touch events
static void on_pointer_callback(ePointer_s pointer, void *ud) {
    if (pointer.action != E_POINTER_DOWN)
        return;
    L.last_click = pointer;
    printf("clicked at x=%f, y=%f, id=%i, is touch: %i\n",
           pointer.pos.x, pointer.pos.y, pointer.id, e_input_is_touch(L.input));
}
//

static void main_loop(float delta_time);

char client_buf[3];
static void client_main() {
    Socket *sock = socket_new("rohl.svenhuis.de", 10000);
    
    Stream_i s = socket_get_stream(sock);
    
    stream_read_msg(s, client_buf, 3);
    
    printf("recv: <%s>\n", client_buf);
}

int main(int argc, char **argv) {
    log_info("some");

    // init e (environment)
    L.window = e_window_new("some");
    L.input = e_input_new(L.window);
    L.gui = e_gui_new(L.window);

    // init r (render)
    L.render = r_render_new(e_window_get_sdl_window(L.window));

    // init systems
    L.camera = camera_new();


    client_main();
    // example code
    // update camera to  init camera.left, ...
    ivec2 window_size = e_window_get_size(L.window);
    camera_update(&L.camera, window_size.x, window_size.y);
    // class init of RoText
    // RoText *self, int max_chars, const float *camera_vp_matrix
    L.text = ro_text_new_font55(128);
    // see u/pose.h, sets a mat4 transformation pose
    u_pose_set_xy(&L.text.pose, L.camera.RO.left + 5, 10);
    
    // font 85 contains full ascii chars
    L.text85 = ro_text_new_font85(128);
    u_pose_set_xy(&L.text85.pose, L.camera.RO.left + 5, -30);

    // setup a pointer listener
    e_input_register_pointer_event(L.input, on_pointer_callback, NULL);

    // set clear color
    *r_render_clear_color(L.render) = (vec4) {0.5, 0.75, 0.5, 1};
    //
    
    e_window_main_loop(L.window, main_loop);

    r_render_kill(&L.render);
    e_gui_kill(&L.gui);
    e_input_kill(&L.input);
    e_window_kill(&L.window);

    return 0;
}


static void main_loop(float delta_time) {
    ivec2 window_size = e_window_get_size(L.window);

    // e updates
    e_input_update(L.input);

    // simulate
    camera_update(&L.camera, window_size.x, window_size.y);
    mat4 *camera_mat = &L.camera.matrices.vp;


    // render
    r_render_begin_frame(L.render, window_size.x, window_size.y);


    // example code
    static float val = 10;
    //creates a debug window to set val
    // min, max, step
    e_gui_wnd_float_attribute(L.gui, "val", &val, 0, 100, 5);
    char buf[128];
    snprintf(buf, 128, "Hello World\nval=%5.1f\nspace pressed: %i\nid=%i x=%.2f y=%.2f\nrecv: <%s>" ,
             val, e_input_get_keys(L.input).space, L.last_click.id, L.last_click.pos.x, L.last_click.pos.y, client_buf);
    // RoText methods: set text, render
    ro_text_set_text(&L.text, buf);
    ro_text_render(&L.text, camera_mat);
    
    static vec3 hsv = {{0, 1, 1}};
    hsv.v0 += delta_time*90;
    
    snprintf(buf, 128, "#include <stdio.h>\nint main() {\n  puts(\"Hello World\");\n}");
    for(int i=0; i<128; i++) {
       vec3 tmp = hsv;
       tmp.v0 = sca_mod(tmp.v0 + i*3, 360);
       L.text85.ro.rects[i].color.xyz = vec3_hsv2rgb(tmp);
    }
    ro_text_set_text(&L.text85, buf);
    ro_text_render(&L.text85, camera_mat);
    //


    // uncomment to clone the current framebuffer into r_render.framebuffer_tex
    // r_render_blit_framebuffer(e_window.size.x, e_window.size.y);

    e_gui_render(L.gui);

    // swap buffers
    r_render_end_frame(L.render);
}


