#ifndef MJPEG2JPEG_H
#define MJPEG2JPEG_H

#include <stdint.h> 

#ifdef __cplusplus
extern "C" {
#endif

int mjpeg2jpeg(uint8_t **poutbuf, int *poutbuf_size, const uint8_t *buf, int buf_size);

#ifdef __cplusplus
}
#endif

#endif // MJPEG2JPEG_H
