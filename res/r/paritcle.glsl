#ifdef VERTEX
    #ifdef OPTION_GLES
        #version 300 es
    #else
        #version 330 core
    #endif

    layout(location = 0) in mat4 in_pose;
    // uses location [0:4] (for each col)

    layout(location = 4) in mat4 in_uv;
    // uses location [4:7] (for each col)

    layout(location = 8) in vec4 in_color;
    
    layout(location = 9) in vec4 in_speed;
    layout(location = 10) in vec4 in_acc;
    layout(location = 11) in vec4 in_axis_angle;
    layout(location = 12) in vec4 in_color_speed;

    layout(location = 13) in vec3 in_uv_step_and_time;

    layout(location = 14) in float start_time;

    // on some Browsers (Chrome on Desktop?!?) location = 15 is not valid, even if GL_MAX_VERTEX_ATTRIBS == 16

    out vec2 v_tex_coord;
    out vec4 v_color;

    uniform mat4 vp;
    uniform float time;

    const vec4 vertices[6] = vec4[](
    vec4(-0.5, -0.5, 0, 1),
    vec4(+0.5, -0.5, 0, 1),
    vec4(-0.5, +0.5, 0, 1),
    vec4(-0.5, +0.5, 0, 1),
    vec4(+0.5, -0.5, 0, 1),
    vec4(+0.5, +0.5, 0, 1)
    );

    // 0-1 may overlap, so using 0-0.9999999 instead
    const vec4 tex_coords[6] = vec4[](
    vec4(0.0000000, 0.9999999, 0, 1),
    vec4(0.9999999, 0.9999999, 0, 1),
    vec4(0.0000000, 0.0000000, 0, 1),
    vec4(0.0000000, 0.0000000, 0, 1),
    vec4(0.9999999, 0.9999999, 0, 1),
    vec4(0.9999999, 0.0000000, 0, 1)
    );


    // axis must be normalized
    mat4 axis_angle_to_rot_mat(vec3 axis, float angle) {
        // equation from: https://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
        float c = cos(angle);
        float s = sin(angle);
        float t = 1.0f - c;
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;
        return mat4(
        t*x*x+c, t*x*y+z*s, t*x*z-y*s, 0, // col 0=x_axis
        t*x*y-z*s, t*y*y+c, t*y*z+x*s, 0, // col 1=y_axis
        t*x*z+y*s, t*y*z-x*s, t*z*z+c, 0, // col 2=z_axis
        0, 0, 0, 1
        );
    }


    void main() {
        float dt = time - start_time;

        vec4 pos = vertices[gl_VertexID];


        pos = axis_angle_to_rot_mat(in_axis_angle.xyz, in_axis_angle.w * dt) * pos;

        pos = in_pose * pos;
        pos += in_speed * dt;
        pos += in_acc * (dt * dt);
        gl_Position = vp * pos;

        vec2 uv_step = in_uv_step_and_time.xy * floor(dt / in_uv_step_and_time.z);
        v_tex_coord = (in_uv * tex_coords[gl_VertexID]).xy + uv_step;

        v_color = in_color + (in_color_speed * dt);
    }

#endif


#ifdef FRAGMENT
    #ifdef OPTION_GLES
        #version 300 es
        precision mediump float;
        precision lowp sampler2DArray;
    #else
        #version 330 core
    #endif

    in vec2 v_tex_coord;
    in vec4 v_color;

    out vec4 out_frag_color;

    uniform sampler2DArray tex;

    void main() {
        vec3 t;
        t.xy = v_tex_coord;
        t.z = 2.0f;
        out_frag_color = texture(tex, t) * v_color;
    }
#endif
