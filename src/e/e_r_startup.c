#include "r/render.h"
#include "r/rect.h"
#include "r/texture.h"
#include "r/ro_text.h"
#include "r/ro_particle.h"
#include "u/pose.h"
#include "mathc/utils/random.h"
#include "mathc/utils/camera.h"
#include "rhc/log.h"

#define CAMERA_SIZE 180 // *4=720; *6=1080; *8=1440
#define TIME_REMAINING 1.0

#define AUTHOR_SIZE 2.0
#define PARTICLES_SIZE 1.0
#define PARTICLES_NUM 2048

// protected
extern rRender *r_render_singleton_;

//
// private
//

typedef void (*e_window_main_loop_fn)(float delta_time);

typedef void (*on_startup_finished_fun)();

static struct {
    ivec2 wnd_size;
    on_startup_finished_fun on_startup_finished;
    float time;

    RoText author_text;
    RoParticle particles;
} singleton;


static void main_loop(float delta_time);


//
// protected
//

e_window_main_loop_fn e_r_startup(const char *author, ivec2 wnd_size, on_startup_finished_fun on_startup_finished) {
    singleton.wnd_size = wnd_size;
    singleton.on_startup_finished = on_startup_finished;
    singleton.time = 0;

    singleton.author_text = ro_text_new_font85(32);
    vec2 text_size = ro_text_set_text(&singleton.author_text, author);
    u_pose_set_xy(&singleton.author_text.pose, -text_size.x/2, 0);
    singleton.author_text.pose = u_pose_new(-text_size.x * AUTHOR_SIZE / 2, 0, AUTHOR_SIZE, AUTHOR_SIZE);
    ro_text_set_color(&singleton.author_text, R_COLOR_WHITE);

    singleton.particles = ro_particle_new(PARTICLES_NUM, r_texture_new_white_pixel());

    for(int i=0; i<PARTICLES_NUM; i++) {
        rParticleRect_s r = r_particlerect_new();
        r.pose = u_pose_new(sca_random_noise(0, CAMERA_SIZE),
                            sca_random_noise(0, CAMERA_SIZE),
                            PARTICLES_SIZE, PARTICLES_SIZE);
        r.speed.y = sca_random_noise(-10, 5);
        r.speed.x = sca_random_noise(0, 2);
        r.color.a = 0.5f;
        singleton.particles.rects[i] = r;
    }
    ro_particle_update(&singleton.particles);

    return main_loop;
}

//
// private
//

static mat4 camera() {
    int wnd_width = singleton.wnd_size.x;
    int wnd_height = singleton.wnd_size.y;
    float smaller_size = wnd_width < wnd_height ? wnd_width : wnd_height;
    float real_pixel_per_pixel = floorf(smaller_size / CAMERA_SIZE);

    float width_2 = wnd_width / (2 *  real_pixel_per_pixel);
    float height_2 = wnd_height / (2 * real_pixel_per_pixel);

    // begin: (top, left) with a full pixel
    // end: (bottom, right) with a maybe splitted pixel
    float left = -floorf(width_2);
    float top = floorf(height_2);
    float right = width_2 + (width_2 - floorf(width_2));
    float bottom = -height_2 - (height_2 - floorf(height_2));
    return mat4_camera_ortho(left, right, bottom, top, -1, 1);
}

static void update_render(float dtime) {
    mat4 cam = camera();
    ro_text_render(&singleton.author_text, &cam);
    ro_particle_render(&singleton.particles, singleton.time, &cam);
}

static void main_loop(float delta_time) {
    singleton.time += delta_time;
    if(singleton.time >= TIME_REMAINING)
        singleton.on_startup_finished();

    r_render_begin_frame(r_render_singleton_, singleton.wnd_size.x, singleton.wnd_size.y);
    update_render(delta_time);
    r_render_end_frame(r_render_singleton_);
    if(r_render_error_check_impl_("e_r_startup")) {
        log_error("Unexpected OpenGL errors occured while checking in startup");
        r_exit_failure();
    }
}
