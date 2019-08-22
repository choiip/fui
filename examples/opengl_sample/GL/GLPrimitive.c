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

__cube*
createCube(unsigned int program)
{
  if (program == 0) return NULL;

  __cube* c = (__cube*)malloc(sizeof(__cube));

  c->program     = program;
  c->mvpLoc      = glGetUniformLocation(program, "u_mvp");
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
