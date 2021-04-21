#ifdef VERTEX

    out vec2 v_tex_coord;
    out vec4 v_color;

    uniform mat4 pose;
    uniform mat4 vp;
    uniform mat4 uv;
    uniform vec4 color;


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

    void main() {
        gl_Position = vp * pose * vertices[gl_VertexID];
        v_tex_coord = (uv * tex_coords[gl_VertexID]).xy;
        v_color = color;
    }
#endif


#ifdef FRAGMENT
    #ifdef OPTION_GLES
        precision mediump float;
        precision lowp sampler2DArray;
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
