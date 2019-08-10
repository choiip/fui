#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct cube;

struct cube*
createCube();

void
drawCube(struct cube* c, const float* mvp);

void
destroyCube(struct cube* c);

#ifdef __cplusplus
}
#endif
