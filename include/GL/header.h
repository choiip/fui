#pragma once

#ifdef __EMSCRIPTEN__
#include <GLES2/gl2platform.h>
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>
#define GL_BGRA GL_BGRA_EXT
#define GL_R8 GL_R8_EXT
#define GL_RED GL_RED_EXT
#define GL_UNPACK_ROW_LENGTH GL_UNPACK_ROW_LENGTH_EXT
#define GL_UNPACK_SKIP_ROWS GL_UNPACK_SKIP_ROWS_EXT
#define GL_UNPACK_SKIP_PIXELS GL_UNPACK_SKIP_PIXELS_EXT

#define glGenVertexArrays glGenVertexArraysOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glIsVertexArray glIsVertexArrayOES
#define glBeginQuery glBeginQueryEXT
#define glEndQuery glEndQueryEXT
#define glGetQueryObjectiv glGetQueryObjectivEXT
#define GL_QUERY_RESULT_AVAILABLE GL_QUERY_RESULT_AVAILABLE_EXT
#else
#include "glad.h"
#endif
