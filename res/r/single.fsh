precision lowp sampler2DArray;

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
