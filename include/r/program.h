#ifndef R_PROGRAM_H
#define R_PROGRAM_H

//
// create and compile shader programs
//

#include <stdbool.h>
#include "core.h"


typedef struct {
    GLint type;
    const char *src_or_file;
} rProgramShaderSource_s;

// compiles a shader
GLuint r_program_shader_new(rProgramShaderSource_s source);

// compiles a shader, src=file
GLuint r_program_shader_new_file(rProgramShaderSource_s file);

// creates a program from given shaders
GLuint r_program_new(const GLuint *shaders, int n, bool delete_shaders);

// loads a program from a single file (only vertex and fragment shader)
GLuint r_program_new_file(const char *file);

// tests if a draw call is valid (heavy call)
void r_program_validate(GLuint program);


#endif //R_PROGRAM_H
