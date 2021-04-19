#include "rhc/error.h"
#include "r/program.h"


static char *file_read(const char *filename) {
    SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
    if (rw == NULL) return NULL;

    Sint64 res_size = SDL_RWsize(rw);
    char *res = (char *) malloc(res_size + 1);

    Sint64 nb_read_total = 0, nb_read = 1;
    char *buf = res;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }
    SDL_RWclose(rw);
    if (nb_read_total != res_size) {
        free(res);
        return NULL;
    }

    res[nb_read_total] = '\0';
    return res;
}

GLuint r_program_shader_new(rProgramShaderSource_s source) {
    const char *type = NULL;
    if(source.type == GL_VERTEX_SHADER)
        type = "#define VERTEX\n";
    if(source.type == GL_FRAGMENT_SHADER)
        type = "#define FRAGMENT\n";

    assume(type, "neither vertex nor fragment shader used: %i", source.type);

    const char *option_gles = "";
#ifdef OPTION_GLES
    option_gles = "#define OPTION_GLES\n";
#endif

    GLint shader = glCreateShader(source.type);

    glShaderSource(shader, 3, (const char *[]) {type, option_gles, source.src_or_file}, NULL);

    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = malloc(log_len + 1);
        glGetShaderInfoLog(shader, log_len, NULL, buffer);
        SDL_Log("Shader compile failure in %s shader: %s",  type, buffer);
        free(buffer);

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint r_program_shader_new_file(rProgramShaderSource_s file) {
    char *source = file_read(file.src_or_file);
    if (!source) {
        SDL_Log("Load shader file %s failed: %s", file.src_or_file, SDL_GetError());
        return 0;
    }

    GLuint shader = r_program_shader_new((rProgramShaderSource_s) {file.type, source});
    free(source);
    return shader;
}

GLuint r_program_new(const GLuint *shaders, int n, bool delete_shaders) {
    GLuint program = 0;

    // Combine shaders into program
    program = glCreateProgram();
    for (int i = 0; i < n; i++)
        glAttachShader(program, shaders[i]);
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = malloc(log_len + 1);
        glGetProgramInfoLog(program, log_len, NULL, buffer);
        SDL_Log("Shader linking failure: %s", buffer);
        free(buffer);

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
    char *source = file_read(file);
    if (!source) {
        SDL_Log("Load program file %s failed: %s", file, SDL_GetError());
        return 0;
    }

    GLuint vertex = r_program_shader_new((rProgramShaderSource_s) {GL_VERTEX_SHADER, source});
    GLuint fragment = r_program_shader_new((rProgramShaderSource_s) {GL_FRAGMENT_SHADER, source});

    GLuint program = 0;
    if(vertex != 0 && fragment != 0) {
        program = r_program_new((const GLuint[]) {vertex, fragment}, 2, false);
    }

    // ok to pass 0
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(source);
    return program;
}

void r_program_validate(GLuint program) {
    glValidateProgram(program);

    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        int log_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

        char *buffer = malloc(log_len + 1);
        glGetProgramInfoLog(program, log_len, NULL, buffer);
        SDL_Log("Program validate failure: %s", buffer);
        free(buffer);
    }
}
