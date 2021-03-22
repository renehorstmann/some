#include "mathc/float.h"
#include "r/shader.h"
#include "r/ro_single.h"


void r_ro_single_init(rRoSingle *self, const float *vp, GLuint tex_sink) {
    self->rect.pose = mat4_eye();
    self->rect.uv = mat4_eye();
    self->rect.color = vec4_set(1);

    self->vp = vp;

    self->program = r_compile_glsl_from_files((char *[]) {
            "res/r/single.vsh",
            "res/r/single.fsh",
            NULL
    });

    self->tex = tex_sink;
    self->owns_tex = true;

    // vao scope
    {
        glGenVertexArrays(1, &self->vao);
        glBindVertexArray(self->vao);

        // texture (using only unit 0)
        glUniform1i(glGetUniformLocation(self->program, "texs"), 0);

        glBindVertexArray(0);
    }
}

void r_ro_single_kill(rRoSingle *self) {
    glDeleteProgram(self->program);
    glDeleteVertexArrays(1, &self->vao);
    if(self->owns_tex)
        glDeleteTextures(1, &self->tex);
    *self = (rRoSingle) {0};
}


#include "r/render.h"

void r_ro_single_render(rRoSingle *self) {
    glUseProgram(self->program);

    GLint loc;

    loc = glGetUniformLocation(self->program, "pose");
    printf("uniforms pose loc: %i\n", loc);
    r_render_error_check();
    puts("set");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &self->rect.pose.m00);
    r_render_error_check();


    loc = glGetUniformLocation(self->program, "vp");
    printf("uniforms vp loc: %i\n", loc);
    r_render_error_check();
    puts("set");
    glUniformMatrix4fv(loc, 1, GL_FALSE, self->vp);
    r_render_error_check();

    loc = glGetUniformLocation(self->program, "uv");
    printf("uniforms uv loc: %i\n", loc);
    r_render_error_check();
    puts("set");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &self->rect.uv.m00);
    r_render_error_check();

    loc = glGetUniformLocation(self->program, "color");
    printf("uniforms color loc: %i\n", loc);
    r_render_error_check();
    puts("set");
    glUniform4fv(loc, 1, &self->rect.color.v0);
    r_render_error_check();

    puts("setting texture and draw");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self->tex);

    {
        glBindVertexArray(self->vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    glUseProgram(0);
}

void r_ro_single_set_texture(rRoSingle *self, GLuint tex_sink) {
    if(self->owns_tex)
        glDeleteTextures(1, &self->tex);
    self->tex = tex_sink;
}
