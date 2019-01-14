#ifndef JPG_DECOMPRESS_H
#define JPG_DECOMPRESS_H

#ifdef __cplusplus
extern "C" {
#endif

int jpg_decompress( const void* srcData,
                    const unsigned int srcSize,
                    void* dst,
                    const unsigned int dstSize,
                    unsigned int* width,
                    unsigned int* height,
                    unsigned int* numChannels );

#ifdef __cplusplus
}
#endif

#endif // JPG_DECOMPRESS_H
