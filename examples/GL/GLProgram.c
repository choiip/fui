#include "GLProgram.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glad.h>

static 
void 
dumpShaderError(GLuint shader, const char* name, const char* type)
{
	GLchar str[512+1];
	GLsizei len = 0;
	glGetShaderInfoLog(shader, 512, &len, str);
	if (len > 512) len = 512;
	str[len] = '\0';
	printf("Shader %s/%s error:\n%s\n", name, type, str);
}

static
void 
dumpProgramError(GLuint prog, const char* name)
{
	GLchar str[512+1];
	GLsizei len = 0;
	glGetProgramInfoLog(prog, 512, &len, str);
	if (len > 512) len = 512;
	str[len] = '\0';
	printf("Program %s error:\n%s\n", name, str);
}

unsigned int
createProgram(const char* name, const char* vertex_shader, const char* fragment_shader) {
  GLint status;
  GLuint vtx_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vtx_shader, 1, &vertex_shader, 0);
  glCompileShader(vtx_shader);
	glGetShaderiv(vtx_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		dumpShaderError(vtx_shader, name, "vert");
		return 0;
	}

  GLuint fgmt_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fgmt_shader, 1, &fragment_shader, 0);
  glCompileShader(fgmt_shader);
	glGetShaderiv(fgmt_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		dumpShaderError(fgmt_shader, name, "fgmt");
		printf("%s", fragment_shader);
		return 0;
	}

  GLuint program = glCreateProgram();

  glAttachShader(program, vtx_shader);
  glAttachShader(program, fgmt_shader);

  glDeleteShader(vtx_shader);
  glDeleteShader(fgmt_shader);

  glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		dumpProgramError(program, name);
    glDeleteProgram(program);
		return 0;
	}
  return program;
}

void
destroyProgram(unsigned int program) {
  glDeleteProgram(program);
}
