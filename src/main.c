#include "e/e.h"
#include "r/r.h"
#include "u/u.h"
#include "rhc/rhc.h"
#include "mathc/mathc.h"

#include "camera.h"

static struct {
    eSimple *simple;
    Camera_s camera;

    //// example code:
    // 'R'ender'o'bject
    // renders text via RoBatch
    RoText text;
    RoText text85;
    // stores the last pressed mouse click / touch to render with RoText text
    ePointer_s last_click;
    ////

} L;


//// example code
// will be called on mouse or touch events
static void on_pointer_callback(ePointer_s pointer, void *ud) {
    if (pointer.action != E_POINTER_DOWN)
        return;
    L.last_click = pointer;
    printf("clicked at x=%f, y=%f, id=%i, is touch: %i\n",
           pointer.pos.x, pointer.pos.y, pointer.id, e_input_is_touch(L.simple->input));
}
////


// this function will be called at the start of the app
static void init(eSimple *simple, ivec2 window_size) {
    // catch eSimple for the pointer callback
    // eSimple is always the same instance
    L.simple = simple;
    
    // init systems
    L.camera = camera_new();


    //// example code
    // update camera to init camera.left, ...
    camera_update(&L.camera, window_size);
    // class init of RoText
    // RoText *self, int max_chars, const float *camera_vp_matrix
    L.text = ro_text_new_font55(128);
    // see u/pose.h, sets a mat4 transformation pose
    // most render objects got their xy positions as a center point, but for text its top left
    u_pose_set_xy(&L.text.pose, L.camera.RO.left + 5, -10);
    // font 85 contains full ascii chars
    L.text85 = ro_text_new_font85(128);
    u_pose_set_xy(&L.text85.pose, L.camera.RO.left + 5, -40);
    // setup a pointer listener
    e_input_register_pointer_event(simple->input, on_pointer_callback, NULL);
    // set clear color = background color
    *r_render_clear_color(simple->render) = (vec4) {0.5, 0.75, 0.5, 1};
    ////
}


// this functions is called either each frame or at a specific update/s time
static void update(eSimple *simple, ivec2 window_size, float delta_time) {
    // simulate
    camera_update(&L.camera, window_size);
    

    //// example code
    static float val = 10;
    // creates a debug window to set val
    // min, max, step
    e_gui_float("val", &val, 0, 100);
    //
    // creates a debug window to set the clear color
    vec4 *clear_color = r_render_clear_color(simple->render);
    e_gui_rgb("background", (vec3*) clear_color);
    //
    // create a text (font55) and render it
    char buf[128];
    snprintf(buf, 128, 
            "Hello World\n"
            "val=%5.1f\n"
            "space pressed: %i\n"
            "id=%i x=%.2f y=%.2f",
             val, e_input_get_keys(simple->input).space, L.last_click.id, L.last_click.pos.x, L.last_click.pos.y);
    // RoText methods: set text, render
    ro_text_set_text(&L.text, buf);
    //
    // render the text (font85) with a rainbow animation with hsv
    static vec3 hsv = {{0, 1, 1}};
    hsv.v0 += delta_time*90;
    //
    snprintf(buf, 128, 
            "#include <stdio.h>\n"
            "int main() {\n"
            "  puts(\"Hello World\");\n"
            "}\n"
            "// some framework\n"
            "// by Horsimann");
    for(int i=0; i<128; i++) {
       vec3 tmp = hsv;
       tmp.v0 = sca_mod(tmp.v0 + i*3, 360);
       // access the texts internal RoBatch's rRect's to change the color of each char
       L.text85.ro.rects[i].color.xyz = vec3_hsv2rgb(tmp);
    }
    ro_text_set_text(&L.text85, buf);
}


// this function is calles each frame to render stuff
static void render(eSimple *simple, ivec2 window_size) {
    mat4 *camera_mat = &L.camera.matrices.vp;
    ro_text_render(&L.text, camera_mat);
    ro_text_render(&L.text85, camera_mat);
    
    
    // uncomment to clone the current framebuffer into r_render_get_framebuffer_tex
    // r_render_blit_framebuffer(simple->render, window_size);
}


int main(int argc, char **argv) {
    e_simple_start("some", "Horsimann", 
            0, // updates/s, <=0 to turn off and use fps
            init, update, render);
    return 0;
}
