#include "rhc/error.h"
#include "rhc/file.h"
#include "r/program.h"


GLuint r_program_shader_new(Str_s source, GLint shader_type) {
    // check source available
    if(str_empty(source))
        return 0;

    const char *type = NULL;
    if(shader_type == GL_VERTEX_SHADER)
        type = "#define VERTEX\n";
    if(shader_type == GL_FRAGMENT_SHADER)
        type = "#define FRAGMENT\n";

    assume(type, "neither vertex nor fragment shader used: %i", shader_type);

    const char *option_gles = "";
#ifdef OPTION_GLES
    option_gles = "#define OPTION_GLES\n";
#endif

    GLint shader = glCreateShader(shader_type);

    glShaderSource(shader, 3,
                   (const char *[]) {type, option_gles, source.data},
                   (GLint[]) {strlen(type), strlen(option_gles), source.size});

    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = rhc_malloc_raising(log_len + 1);
        glGetShaderInfoLog(shader, log_len, NULL, buffer);
        SDL_Log("Shader compile failure in %s shader: %s",  type, buffer);
        rhc_free(buffer);

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint r_program_shader_new_file(const char *file, GLint shader_type) {
    String source = file_read(file, true);
    GLuint shader = r_program_shader_new(source.str, shader_type);
    string_kill(&source);
    return shader;
}

GLuint r_program_new(const GLuint *shaders, int n, bool delete_shaders) {
    // check each shader valid
    for(int i=0; i<n; i++) {
        if(!r_program_shader_valid(shaders[i]))
            return 0;
    }

    // Combine shaders into program
    GLuint program = glCreateProgram();
    for (int i = 0; i < n; i++)
        glAttachShader(program, shaders[i]);
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = rhc_malloc_raising(log_len + 1);
        glGetProgramInfoLog(program, log_len, NULL, buffer);
        SDL_Log("Shader linking failure: %s", buffer);
        rhc_free(buffer);

        glDeleteProgram(program);
        program = 0;
    }

    if(delete_shaders) {
        for (int i = 0; i < n; i++)
            glDeleteShader(shaders[i]);
    }

    return program;
}

GLuint r_program_new_file(const char *file) {
    String source = file_read(file, true);
    GLuint vertex = r_program_shader_new(source.str, GL_VERTEX_SHADER);
    GLuint fragment = r_program_shader_new(source.str, GL_FRAGMENT_SHADER);

    GLuint program = r_program_new((const GLuint[]) {vertex, fragment}, 2, false);

    // safe to pass 0
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    string_kill(&source);

    return program;
}

void r_program_validate(GLuint program) {
    if(!r_program_valid(program))
        return;

    glValidateProgram(program);

    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = rhc_malloc_raising(log_len + 1);
        glGetProgramInfoLog(program, log_len, NULL, buffer);
        SDL_Log("Program validate failure: %s", buffer);
        rhc_free(buffer);
    }
}
