#include "GLPrimitive.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glad.h>

typedef struct cube 
{
   GLuint program;
   GLint  mvpLoc;
   GLint  positionLoc;
   GLint  colorLoc;
   GLuint vertexBufferID;
   GLuint vertexArrayID;   
} __cube;

static const float vertices[] =
{
  // Front
  -0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,
  // Right
  0.5f, 0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, 0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, -0.5f,
  // Back
  0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  // Left
  -0.5f, 0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, 0.5f,
  -0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, -0.5f, 0.5f,
  // Top
  -0.5f, 0.5f, -0.5f,
  -0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,
  -0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  // Bottom
  -0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,

  // Front
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  // Right
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  // Back
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  // Left
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  0.0625f, 0.57421875f, 0.92578125f, 1.0f,
  // Top
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  0.29296875f, 0.66796875f, 0.92578125f, 1.0f,
  // Bottom
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f,
  0.52734375f, 0.76171875f, 0.92578125f, 1.0f
};

static const char vertex_shader[] =
  "#version 150 core\n"
  "uniform mat4 u_mvpMat;"
  "in vec4 a_position;"
  "in vec4 a_color;"
  "out vec4 v_color;"
  "void main()"
  "{"
  " v_color = a_color;"
  " gl_Position = u_mvpMat * a_position;"
  "}";

static const char fragment_shader[] =
  "#version 150 core\n"
	"#ifdef GL_ES\n"
  " precision mediump float;\n"
  "#endif\n"
  "in vec4 v_color;"
  "out vec4 outColor;"
  "void main()"
  "{"
    "outColor = v_color;"
  "}";
   
static void dumpShaderError(GLuint shader, const char* name, const char* type)
{
	GLchar str[512+1];
	GLsizei len = 0;
	glGetShaderInfoLog(shader, 512, &len, str);
	if (len > 512) len = 512;
	str[len] = '\0';
	printf("Shader %s/%s error:\n%s\n", name, type, str);
}

static void dumpProgramError(GLuint prog, const char* name)
{
	GLchar str[512+1];
	GLsizei len = 0;
	glGetProgramInfoLog(prog, 512, &len, str);
	if (len > 512) len = 512;
	str[len] = '\0';
	printf("Program %s error:\n%s\n", name, str);
}

static unsigned int
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

__cube*
createCube()
{
  GLuint program = createProgram("cube", vertex_shader, fragment_shader); 
  if (program == 0) return NULL;

  __cube* c = (__cube*)malloc(sizeof(__cube));

  c->program     = program;
  c->mvpLoc      = glGetUniformLocation(program, "u_mvpMat");
  c->positionLoc = glGetAttribLocation(program, "a_position");
  c->colorLoc    = glGetAttribLocation(program, "a_color");

  glGenVertexArrays(1, &c->vertexArrayID);
  glBindVertexArray(c->vertexArrayID);

  glGenBuffers(1, &c->vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, c->vertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(c->positionLoc);
  glVertexAttribPointer(c->positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  glEnableVertexAttribArray(c->colorLoc);
  glVertexAttribPointer(c->colorLoc, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(36 * 3 * sizeof(float)));
  
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return c;
}

void
drawCube(__cube* c, const float* mvp) {
  if (c == NULL) return;
  glUseProgram(c->program);
  glBindVertexArray(c->vertexArrayID);
  glUniformMatrix4fv(c->mvpLoc, 1, GL_FALSE, mvp);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void destroyCube(__cube* c) {
  //TODO: free opengl resources
  free(c);
}
