#pragma once

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
createProgram(const char* name, const char* vertex_shader, const char* fragment_shader);

void
destroyProgram(unsigned int program);

#ifdef __cplusplus
}
#endif